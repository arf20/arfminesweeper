/*

  Copyright (C) 2023 Ángel Ruiz Fernandez

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, version 3.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program.  If not, see
  <http://www.gnu.org/licenses/>

  fbdev.c: fbdev frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "common.h"
#include "../game.h"
#include "fbdev.h"

static const int *board = NULL;
static int size = 0;

static int fbfd = 0, screensize = 0;

typedef struct fbpix_s {
    char r, g, b, a;
} fbpix_t;

static int sWidth = 0, fbWidth = 0, sHeight = 0;
static fbpix_t *fbp = NULL; /* assume 32bpp */
#define FB_XY(x, y)  fbp[(y * fbWidth) + x]

/* assume 32bpp */
fbpix_t fbColor(char r, char g, char b, char a) {
    fbpix_t c;
    c.r = r; c.g = g; c.b = b; c.a = a;
    return c;
}

#define FB_WHITE    fbColor(0xff, 0xff, 0xff, 0xff)

/* fb utils */
void
fbClear() {
    memset(fbp, 0, sWidth * sHeight);
}

void
fbFillRect(int ix, int iy, int w, int h, fbpix_t c) {
    if (ix < 0 || iy < 0 || ix + w >= sWidth || iy + h >= sHeight) return;
    for (int y = iy; y < iy + h; y++)
        for (int x = ix; x < ix + w; x++)
            FB_XY(x, y) = c;
}

static void
drawFlag(int x, int y) {
    
}

static void
render() {
    static char buff[256];

    fbClear();

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
            /* If clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                int n = gameGetSurroundingMines(x, y);
                if (n) {
                    snprintf(buff, 256, "%d", n);
                    switch (n) {
                        
                    }
                    //XDrawString(d, w, gc, cX + TXT_OFFX, cY + TXT_OFFY,
                    //    buff, strlen(buff));
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                fbFillRect(cX, cY, CELL_SIZE, CELL_SIZE, FB_WHITE);
                drawFlag(cX, cY);
            }
            /* Otherwise just a tile */
            else {
                fbFillRect(cX, cY, CELL_SIZE, CELL_SIZE, FB_WHITE);
            }
        }
    }
}

int
fbdevStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    /* Through the power of linux magic, open the framebuffer device and map it
        to memory */
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        printf("Error opening /dev/fb0: %s\n", strerror(errno));
        return -1;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        printf("Error reading fb variable information: %s\n", strerror(errno));
        return -1;
    }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        printf("Error reading fb variable information: %s\n", strerror(errno));
        return -1;
    }

    sWidth = vinfo.xres; sHeight = vinfo.yres;
    fbWidth = finfo.line_length / sizeof(int);
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    const int PADDING = 4096;
    int mmapsize = (screensize + PADDING - 1) & ~(PADDING-1);

    fbp = (fbpix_t*)mmap(0, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        printf("Error mapping fbdev to memory: %s\n", strerror(errno));
        return -1;
    }

    printf("fb0: %d(%d)x%d, %dbpp\n", vinfo.xres, fbWidth, vinfo.yres,
        vinfo.bits_per_pixel);

    render();
}

void
fbdevDestroy() {
    munmap(fbp, screensize);
    close(fbfd);
}
