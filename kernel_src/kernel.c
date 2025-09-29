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

    kernel.c: kernel main

*/

#include "plibc.h"
#include "keyb.h"
#include "textdefs.h"
#include "alloc.h"
#include "console.h"

#include <stdint.h>

#include "../common/game.h"
#include "kfrontends/vgacli.h"
#include "kfrontends/vgatui.h"
#include "kfrontends/vgagra.h"

#include "multiboot2.h"

void
kmain(unsigned long mbmagic, unsigned long mbiaddr) {
    /* Defaults */
    unsigned char vgamode = 0x03, vgagmode = 0x13, vgafont = 0x04;
    unsigned int fbwidth = 0, fbheight = 0;
    unsigned char vga = 1;

    int size = 8, mines = 10;
    char ibuf[256];
    memset(ibuf, 0, 256);

    /* parse multiboot data */
    if (mbmagic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        kprintf("Invalid magic number: 0x%x\n", (unsigned int)mbmagic);
        goto cold_start;
    }

    if (mbiaddr & 7) {
        kprintf("Unaligned mbi: 0x%x\n", mbiaddr);
        return;
    }

    unsigned int mbisize = *(unsigned int*)mbiaddr;
    struct multiboot_tag *tag = NULL;
    for (tag = (struct multiboot_tag *)(mbiaddr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
        + ((tag->size + 7) & ~7)))
    {
        switch (tag->type) {
        /* select vga or fb UI */
        case MULTIBOOT_TAG_TYPE_CMDLINE: {
            const char *cmdline = ((struct multiboot_tag_string*)tag)->string;
            if (strcmp(cmdline, "vga") == 0)
                vga = 1;
            else if (strcmp(cmdline, "fb") == 0)
                vga = 0;
            else
                vga = 1;
        } break;
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
            unsigned i;
            struct multiboot_tag_framebuffer *tagfb
                = (struct multiboot_tag_framebuffer*)tag;
            void *fb = (void*)(unsigned long)tagfb->common.framebuffer_addr;

            switch (tagfb->common.framebuffer_type) {
            case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                /* support only 8888-BGRX framebuffers */
                if (tagfb->common.framebuffer_bpp == 32 &&
                    tagfb->framebuffer_red_field_position == 16 &&
                    tagfb->framebuffer_red_mask_size == 8 &&
                    tagfb->framebuffer_green_field_position == 8 &&
                    tagfb->framebuffer_green_mask_size == 8 &&
                    tagfb->framebuffer_blue_field_position == 8 &&
                    tagfb->framebuffer_blue_mask_size == 8
                ) {
                    vga = vga || 0;
                    fbwidth = tagfb->common.framebuffer_width;
                    fbheight = tagfb->common.framebuffer_height;
                } else {
                    vga = 1;
                    kprintf("Unsupported framebuffer\n");
                }
            break;
            case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                vga = 1;
            break;
            default:
                vga = 1;
                kprintf("Framebuffer not supported\n");
            break;
            }
        }
        }
    }

cold_start:
    /* clear screen, set up terminal */
    if (vga)
        con_init_vga(vgamode, vgafont);
    else
        con_init_fb(fbwidth, fbheight);

    /* set heap at the start of Extended Memory (>1MiB), 1MiB in size */
    alloc_init((void*)0x00100000, (void*)0x001fffff);

warm_start:
    con_clear();
    kprintf("%s\n%s", TXT_HELLO, TXT_MENU);
    kprintf("\nCurrent config: %dx%d size, %d mines; ", size, size, mines);
    if (vga)
        kprintf("vga text mode %2Xh, vga font %2Xh, vga graphic mode %2Xh\n",
            vgamode, vgagmode);
    else
        kprintf("fb 8888BGRX width %d height %d\n", fbwidth, fbheight);

    while (1) {
        char sel = keyb_getc();
        
        switch (sel) {
            case 'h': return;
            case 's': {
                kprintf("size> ");
                getsn(ibuf, 256);
                size = atoi(ibuf);
                goto warm_start;
            } break;
            case 'm': {
                kprintf("mines> ");
                getsn(ibuf, 256);
                mines = atoi(ibuf);
                goto warm_start;
            } break;
            case 'v': {
                con_clear();
                kprintf(TXT_TEXT_MODES);
                getsn(ibuf, 256);
                vgamode = strtol(ibuf, NULL, 16);
                goto cold_start;
            } break;
            case 'f': {
                con_clear();
                kprintf(TXT_FONT);
                getsn(ibuf, 256);
                vgafont = strtol(ibuf, NULL, 16);
                goto cold_start;
            } break;
            case 'g': {
                con_clear();
                kprintf(TXT_GRAPHIC_MODES);
                getsn(ibuf, 256);
                vgagmode = strtol(ibuf, NULL, 16);
                goto warm_start;
            } break;

            case '1': {
                kprintf("Starting game with vgacon frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgacli_start(gameGetBoard(), size);
                goto warm_start;
            } break;
            case '2': {
                kprintf("Starting game with vgatxt frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgatui_start(gameGetBoard(), size, 0);
                goto warm_start;
            } break;
            case '3': {
                kprintf("Starting game with vgatgr frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgatui_start(gameGetBoard(), size, 1);
                goto warm_start;
            } break;
            case '4': {
                kprintf("Starting game with vgagra frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgagra_start(gameGetBoard(), size, vgagmode);
                goto cold_start;
            } break;
            default: break;
        }
    }
}
