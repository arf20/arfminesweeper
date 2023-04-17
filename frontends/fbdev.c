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
#include "bmp.h"

static const int *board = NULL;
static int size = 0;

static int fbfd = 0, screensize = 0;

static int sWidth = 0, fbWidth = 0, sHeight = 0;
static rgba_t *fbp = NULL; /* assume 32bpp */
#define FB_XY(x, y)  fbp[(y * fbWidth) + x]

/* assume 9x15 font */
const rgba_t *fontimg = NULL;
int fontiw = 0, fontih = 0;
#define FONT_W  9
#define FONT_H  15
#define FONT_XY(x, y)  fontimg[(y * fontiw) + x]

/* assume 32bpp */
rgba_t fbColor(char r, char g, char b, char a) {
    rgba_t c;
    c.r = r; c.g = g; c.b = b; c.a = a;
    return c;
}

#define FB_WHITE    fbColor(0xff, 0xff, 0xff, 0xff)
#define FB_BLACK    fbColor(0x00, 0x00, 0x00, 0xff)

/* fb utils */
void
fbClear() {
    memset(fbp, 0, sWidth * sHeight);
}

void
fbFillRect(int ix, int iy, int w, int h, rgba_t c) {
    if (ix < 0 || iy < 0 || ix + w >= sWidth || iy + h >= sHeight) return;
    for (int y = iy; y < iy + h; y++)
        for (int x = ix; x < ix + w; x++)
            FB_XY(x, y) = c;
}

void
fbCopy(int dx, int dy, int sx, int sy, int w, int h, const rgba_t *src) {
    if (dx < 0 || dy < 0 || dx + w >= sWidth || dy + h >= sHeight) return;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            FB_XY(dx + x, dy + y) = FONT_XY(sx + x, sy + y);
}

void
fbDrawString(int ix, int iy, rgba_t fg, rgba_t bg, const char *str) {
    const char *ptr = str;
    int i = 0;
    while (*ptr) {
        i = ptr - str;
        fbCopy((FONT_W * i) + ix, iy, FONT_W * *ptr, 0, FONT_W, FONT_H, fontimg);
        ptr++;
    }
}

static void
drawFlag(int x, int y) {
    
}

static void
render() {
    static char buff[256];

    fbClear();
    fbDrawString(5, 0, FB_WHITE, FB_BLACK, TXT_TITLE);
    fbCopy(100, 100, 0, 0, 100, 15, fontimg);

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

    /* Read bitmap font */
    if (readBMP("../charstrip.bmp", &fontimg, &fontiw, &fontih) != 0) {
        return -1;
    }

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

    fbp = (rgba_t*)mmap(0, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
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
    free((void*)fontimg);
    munmap(fbp, screensize);
    close(fbfd);
}
