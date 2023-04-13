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

static int sWidth = 0, sHeight = 0;
static int *fbp = NULL; /* assume 32bpp */
#define FBXY(x, y)  fbp[(y * sWidth) + x]

static void
render() {
    FBXY(500, 500) = 0xffffffff;
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
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        printf("Error reading fb variable information: %s\n", strerror(errno));
        return -1;
    }

    printf("fb0: %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    sWidth = vinfo.xres; sHeight = vinfo.yres;
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    const int PADDING = 4096;
    int mmapsize = (screensize + PADDING - 1) & ~(PADDING-1);

    fbp = (int*)mmap(0, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        printf("Error mapping fbdev to memory: %s\n", strerror(errno));
        return -1;
    }

    render();
}

void
fbdevDestroy() {
    munmap(fbp, screensize);
    close(fbfd);
}
