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

    fbdev.c: fbdev frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <termios.h>

#include <common/frontconf.h>
#include <common/game.h>


#define STB_IMAGE_IMPLEMENTATION
#include <common/stb_image.h>

#include <common/fb.h>

#include "fbdev.h"

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

static const unsigned char *font = NULL;
static const unsigned char *flag = NULL;

static int fbfd = 0;
static bgra_t *fbp = NULL; /* assume 32bpp */
static int screensize = 0;

static struct termios orig;

int
fbdev_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    /* Read bitmap font */
    const unsigned char *font = NULL;
    int fontw = 0, fonth = 0, ch = 0;
    font = stbi_load(FONT_BMP_PATH, &fontw, &fonth, &ch, 1);
    if (!font) {
        printf("Error loading bitmap font: " FONT_BMP_PATH "\n");
        return -1;
    }
    printf("Bitmap font: %dx%d, %dch\n", fontw, fonth, ch);

    /* Read flag bitmap */
    int flagw = 0, flagh = 0;
    flag = stbi_load(FLAG_PNG_PATH, &flagw, &flagh, &ch, 1);
    if (!flag) {
        printf("Error loading flag: " FLAG_PNG_PATH "\n");
        return -1;
    }
    printf("Bitmap flag: %dx%d, %dch\n", flagw, flagh, ch);

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

    static int sWidth = 0, fbWidth = 0, sHeight = 0;

    sWidth = vinfo.xres; sHeight = vinfo.yres;
    fbWidth = finfo.line_length / sizeof(int);
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    const int PADDING = 4096;
    int mmapsize = (screensize + PADDING - 1) & ~(PADDING-1);

    fbp = (bgra_t*)mmap(0, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        printf("Error mapping fbdev to memory: %s\n", strerror(errno));
        return -1;
    }

    printf("fb0: %d(%d)x%d, %dbpp\n", vinfo.xres, fbWidth, vinfo.yres,
        vinfo.bits_per_pixel);

    /* Set terminal to non-canonical non-blocking mode mode, this may fuck it up */
    /* term stuff */
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig);
    raw = orig;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);

    /* cursor */
    int curx = 0, cury = 0;

    fbRenderInit(board, size, wWidth, wHeight, fbp, sWidth, sHeight, font, fontw, fonth, flag, flagw, flagh, &curx, &cury);

    fbRender();
    char input[8] = { 0 };
    int bytesread;
    while (1) {
        do {
            bytesread = read(STDIN_FILENO, &input, 8);
            usleep(10000);
        } while (bytesread <= 0);

        for (int i = 0; i < bytesread; i++) {
            if (input[i] == '\033') {
                if (strncmp(input + i, "\033[A", 3) == 0) cury--;
                if (strncmp(input + i, "\033[B", 3) == 0) cury++;
                if (strncmp(input + i, "\033[C", 3) == 0) curx++;
                if (strncmp(input + i, "\033[D", 3) == 0) curx--;
                i += 2;
            } else if (isalpha(input[i])) {
                input[i] = tolower(input[i]);
                switch (input[i]) {
                    case 'a': curx--; break;
                    case 'd': curx++; break;
                    case 'w': cury--; break;
                    case 's': cury++; break;
                    case 'f': gameFlagCell(curx, cury); break;
                    case 'c': gameClearCell(curx, cury); break;
                }
            }
            if (curx < 0) curx = size - 1;
            if (cury < 0) cury = size - 1;
            if (curx >= size) curx = 0;
            if (cury >= size) cury = 0;
        }

        fbRender();
        memset(input, 0, 8);
    }
}

void
fbdev_destroy() {
    free((void*)font);
    free((void*)flag);
    munmap(fbp, screensize);
    close(fbfd);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}

const char *
fbdev_name() {
    return "fbdev";
}

