/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    module.c: Linux kernel module entry point

*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/moduleparam.h>

#include "../common/game.h"
#include "../common/frontconf.h"

/* Module data */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("arf20");
MODULE_DESCRIPTION("arfminesweeper linux kernel module");
MODULE_VERSION("0.1");

/* Character device stuff */
#define DEVICE_NAME "arfminesweeper"
#define CLASS_NAME  "arfminesweeper_class"

static int major_number = 0;
static struct class* arfminesweeper_class = NULL;
static struct device* arfminesweeper_device = NULL;

#define RBUF_SIZE   16384   /* 16K read buffer */
#define WBUF_SIZE   256     /* 256B read buffer */
static char read_buff[RBUF_SIZE] = { 0 };
static size_t read_size = 0;

/* Game stuff */
static int size = 8;
static int mines = 10;

static const int *board = NULL;

/* Module parameters */
module_param(size, int, 0);
MODULE_PARM_DESC(size, "Board side size");
module_param(mines, int, 0);
MODULE_PARM_DESC(mines, "Number of mines");

/* File operations */
static int
arfmm_open(struct inode *inodep, struct file *filep) {
	/* printk(KERN_INFO "arfminesweeper: device opened\n"); */
	return 0;
}

static int
arfmm_release(struct inode *inodep, struct file *filep) {
	/* printk(KERN_INFO "arfminesweeper: device closed\n"); */
	return 0;
}

static ssize_t
arfmm_read(struct file *filep, char *buf, size_t len, loff_t *off) {
    int errc = 0;
    size_t to_read_n = min((size_t)(read_size - *off), len);

    if (to_read_n <= 0)
        return 0;

    errc = copy_to_user(buf, read_buff + *off, to_read_n);
    if (errc != 0) {
        printk(KERN_INFO "arfminesweeper: %d chars read faulted\n", errc);
        return -EFAULT;
    }

    printk(KERN_INFO "arfminesweeper: %zu chars read\n", read_size);

    *off += to_read_n;
    return to_read_n;
}

static size_t
render_rbuf(void) {
    char *cur = read_buff;

    switch (gameGetState()) {
        case STATE_LOST: return snprintf(cur, RBUF_SIZE, TXT_LOST); break;
        case STATE_WON:  return snprintf(cur, RBUF_SIZE, TXT_WON);  break;
    }

    cur += snprintf(cur, RBUF_SIZE, "+");
    for (int x = 0; x < size; x++)
        cur += snprintf(cur, RBUF_SIZE, "-");
    cur += snprintf(cur, RBUF_SIZE, "+\n");

    for (int y = 0; y < size; y++) {
        cur += snprintf(cur, RBUF_SIZE, "|");
        for (int x = 0; x < size; x++) {
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                /* If clear, count surrounding cells and print n of mines */
                int n = gameGetSurroundingMines(x, y);
                n ? (cur += snprintf(cur, RBUF_SIZE, "%d", n))
                    : (cur += snprintf(cur, RBUF_SIZE, " "));
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                cur += snprintf(cur, RBUF_SIZE, "F");
            }
            else cur += snprintf(cur, RBUF_SIZE, "#"); /* uncleared */
        }
        cur += snprintf(cur, RBUF_SIZE, "|\n");
    }

    cur += snprintf(cur, RBUF_SIZE, "+");
    for (int x = 0; x < size; x++)
        cur += snprintf(cur, RBUF_SIZE, "-");
    cur += snprintf(cur, RBUF_SIZE, "+\n");

    return strlen(read_buff);
}

static ssize_t
arfmm_write(struct file *filep, const char *buf, size_t len, loff_t *off) {
    char cmd_str[WBUF_SIZE] = { 0 }, c = 0;
    int errc = 0, x = 0, y = 0;

    if (len > WBUF_SIZE) {
        printk(KERN_INFO "arfminesweeper: command too log\n");
        return -EFAULT;
    }

    errc = copy_from_user(cmd_str, buf, len);
    if (errc != 0) {
        printk(KERN_INFO "arfminesweeper: %d chars written faulted\n",   
            errc);
        return -EFAULT;
    }

    if (sscanf(cmd_str, "%c %d %d", &c, &x, &y) != 3) {
        printk(KERN_INFO "arfminesweeper: bad command\n");
	    return -EINVAL;
    }

    if (x < 0 || y < 0 || x >= size || y >= size) {
        printk(KERN_INFO "arfminesweeper: command out of bounds\n");
        return -EINVAL;
    }

    switch (c) {
        case 'c': gameClearCell(x, y); break;
        case 'f': gameFlagCell(x, y); break;
        default: {
            printk(KERN_INFO "arfminesweeper: unknown command\n");
            return -EINVAL;
        } break;
    }

    read_size = render_rbuf();

    printk(KERN_INFO "arfminesweeper: command ok: %c %d %d\n", c, x, y);

    return len;
}

static struct file_operations fops = {
    .open =     arfmm_open,
    .release =  arfmm_release,
    .read =     arfmm_read,
    .write =    arfmm_write
};

static char *
arfmm_devnode(struct device *dev, umode_t *mode) {
    if (!mode)
        return NULL;
    if (dev->devt == MKDEV(major_number, 0))
        *mode = 0666;
    return NULL;
}

static int __init
arfmm_start(void) {
    /* Register major number for character device */
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "arfminesweeper: register_chrdev failed\n");
        return major_number;
    }

    /* Register device class */
    arfminesweeper_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(arfminesweeper_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "arfminesweeper: class_create failed\n");
        return PTR_ERR(arfminesweeper_class);
    }

    arfminesweeper_class->devnode = arfmm_devnode;

    /* Register device */
    arfminesweeper_device = device_create(arfminesweeper_class, NULL,
        MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(arfminesweeper_device)) {
        class_destroy(arfminesweeper_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "arfminesweeper: device_create failed\n");
        return PTR_ERR(arfminesweeper_device);
    }


    gameInit(size, mines);

    board = gameGetBoard();

    read_size = render_rbuf();

    printk(KERN_INFO "arfminesweeper: Starting game with module frontend, "
        "%dx%d in size with %d mines\n", size, size, mines);

    return 0;
}

static void __exit
arfmm_end(void) {
    device_destroy(arfminesweeper_class, MKDEV(major_number, 0));
    class_unregister(arfminesweeper_class);
    class_destroy(arfminesweeper_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    gameDestroy();
    printk(KERN_INFO "arfminesweeper: end\n");
}

module_init(arfmm_start);

module_exit(arfmm_end);
