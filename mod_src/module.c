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

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#include "../common/game.h"

/* Module data */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("arf20");
MODULE_DESCRIPTION("arfminesweeper linux kernel module");
MODULE_VERSION("0.1");

#define DEVICE_NAME "arfminesweeper"
#define CLASS_NAME  "arfminesweeper_class"

#define MSG_SIZE    1024
static char message[MSG_SIZE] = { 0 };
static size_t message_size = 0;

static int major_number = 0;
static struct class* arfminesweeper_class = NULL;
static struct device* arfminesweeper_device = NULL;


static int size = 0;
static const int *board = NULL;

/* File operations */
static int
arfmm_open(struct inode *inodep, struct file *filep) {
	printk(KERN_INFO "arfminesweeper: device opened\n");
	return 0;
}

static int
arfmm_release(struct inode *inodep, struct file *filep) {
	printk(KERN_INFO "arfminesweeper: device closed\n");
	return 0;
}

static ssize_t
arfmm_read(struct file *filep, char *buf, size_t len, loff_t *off) {
    int errc = copy_to_user(buf, message, message_size);
    if (errc != 0) {
        printk(KERN_INFO "arfminesweeper: %d chars read faulted\n", errc);
        return -EFAULT;
    }

    return 0;
}

static ssize_t
arfmm_write(struct file *filep, const char *buf, size_t len, loff_t *off) {
    int errc = copy_from_user(message, buf, len);
    if (errc != 0) {
        printk(KERN_INFO "arfminesweeper: %d chars written faulted\n",   
            errc);
        return -EFAULT;
    }

    printk(KERN_INFO "arfminesweeper: command: %s\n", message);

    return 0;
}

static struct file_operations fops = {
    .open =     arfmm_open,
    .release =  arfmm_release,
    .read =     arfmm_read,
    .write =    arfmm_write
};

static int __init
arfmm_start(void) {
    snprintf(message, MSG_SIZE, "yikes\n");

    /* Register major number */
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

    /* Register device */
    arfminesweeper_device = device_create(arfminesweeper_class, NULL,
        MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(arfminesweeper_device)) {
        class_destroy(arfminesweeper_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Tic Tac Toe Game: Failed to create the device\n");
        return PTR_ERR(arfminesweeper_device);
    }


    gameInit(8, 10);

    board = gameGetBoard();
    size = 8;

    printk(KERN_INFO "arfminesweeper ok\n");

    return 0;
}

static void __exit
arfmm_end(void) {
    device_destroy(arfminesweeper_class, MKDEV(major_number, 0));
    class_unregister(arfminesweeper_class);
    class_destroy(arfminesweeper_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "arfminesweeper end\n");
}

module_init(arfmm_start);

module_exit(arfmm_end);
