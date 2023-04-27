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

    bmp.c: Load bmp file

*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "bmp.h"

typedef struct bmpheader_s {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} bmpheader_t;

typedef struct bmpinfoheader_s {
    unsigned int biSize;
    unsigned long biWidth;
    unsigned long biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned long biXPelsPerMeter;
    unsigned long biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} bmpinfoheader_t;

int
readBMP(char *fname, const rgba_t **imgdata, int *w, int *h) {
    FILE *f = fopen(fname, "rb");
    if (!f) {
        printf("Error reading %s: %s\n", fname, strerror(errno));
        return -1;
    }

    bmpheader_t bh;
    fread(&bh, sizeof(bh), 1, f);

    if (bh.bfType != 0x4d42) {
        printf("Invalid BMP file type\n");
        fclose(f);
        return -1;
    }

    bmpinfoheader_t ih;
    fread(&ih, sizeof(ih), 1, f);

    fseek(f, bh.bfOffBits, SEEK_SET);

    char *bmpdata = malloc(ih.biSizeImage);
    fread(bmpdata, ih.biSizeImage, 1, f);

    rgba_t *rgbadata = malloc(sizeof(rgba_t) * (ih.biSizeImage / 3));

    /* BGR to RGBA */
    for (int i = 0; i < ih.biSizeImage / 3; i++) {
        rgbadata[i].r = bmpdata[(3 * i) + 2];
        rgbadata[i].g = bmpdata[(3 * i) + 1];
        rgbadata[i].b = bmpdata[3 * i];
        rgbadata[i].a = 255;
    }

    *imgdata = rgbadata;

    free(bmpdata);
    fclose(f);
}
