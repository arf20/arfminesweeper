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
#include "vgacon.h"
#include "textdefs.h"
#include "alloc.h"

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
    int size = 8, mines = 10;
    char ibuf[256];
    memset(ibuf, 0, 256);

    vga_init(vgamode, vgafont);
    vga_clear();

    /* parse multiboot data */
    if (mbmagic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        kprintf("Invalid magic number: 0x%x\n", (unsigned int)mbmagic);
        goto cold_start;
    }

    if (mbiaddr & 7) {
        kprintf("Unaligned mbi: 0x%x\n", mbiaddr);
        return;
    }

    kprintf("multiboot2 tags:\n");

    unsigned int mbisize = *(unsigned int*)mbiaddr;
    struct multiboot_tag *tag = NULL;
    for (tag = (struct multiboot_tag *)(mbiaddr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
        + ((tag->size + 7) & ~7)))
    {
        kprintf("Tag 0x%x, Size 0x%x\n", tag->type, tag->size);
        switch (tag->type) {
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            kprintf("Command line = %s\n",
                ((struct multiboot_tag_string*)tag)->string);
        break;
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            kprintf("Boot loader name = %s\n",
                ((struct multiboot_tag_string*)tag)->string);
        break;
        case MULTIBOOT_TAG_TYPE_MODULE:
            kprintf("Module at 0x%x-0x%x. Command line %s\n",
                ((struct multiboot_tag_module*)tag)->mod_start,
                ((struct multiboot_tag_module*)tag)->mod_end,
                ((struct multiboot_tag_module*)tag)->cmdline);
        break;
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            kprintf("mem_lower = %uKB, mem_upper = %uKB\n",
                ((struct multiboot_tag_basic_meminfo*)tag)->mem_lower,
                ((struct multiboot_tag_basic_meminfo*)tag)->mem_upper);
        break;
        case MULTIBOOT_TAG_TYPE_BOOTDEV:
            kprintf("Boot device 0x%x,%u,%u\n",
                ((struct multiboot_tag_bootdev*)tag)->biosdev,
                ((struct multiboot_tag_bootdev*)tag)->slice,
                ((struct multiboot_tag_bootdev*)tag)->part);
        break;
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
            multiboot_uint32_t color;
            unsigned i;
            struct multiboot_tag_framebuffer *tagfb
                = (struct multiboot_tag_framebuffer*)tag;
            void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;

            kprintf("Frame buffer type = ");

            switch (tagfb->common.framebuffer_type) {
            case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED: {
                kprintf("indexed, ");
                unsigned best_distance, distance;
                struct multiboot_color *palette;
            
                palette = tagfb->framebuffer_palette;

                color = 0;
                best_distance = 4*256*256;
            
                for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++) {
                    distance = (0xff - palette[i].blue) 
                        * (0xff - palette[i].blue)
                        + palette[i].red * palette[i].red
                        + palette[i].green * palette[i].green;
                    if (distance < best_distance) {
                        color = i;
                        best_distance = distance;
                    }
                }
            } break;
            case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                kprintf("rgb, ");
                color = ((1 << tagfb->framebuffer_blue_mask_size) - 1) 
                    << tagfb->framebuffer_blue_field_position;
            break;
            case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                kprintf("ega text, ");
                color = '\\' | 0x0100;
            break;
            default:
                color = 0xffffffff;
            break;
            }
            
            kprintf("bpp = %d, pitch = %d, size = %dx%d\n",
                tagfb->common.framebuffer_bpp,
                tagfb->common.framebuffer_pitch,
                tagfb->common.framebuffer_width,
                tagfb->common.framebuffer_height
            );
        }
        }
    }
    tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
        + ((tag->size + 7) & ~7));
    kprintf("Total mbi size 0x%x\nPress any key to continue", (unsigned)tag - mbiaddr);

    getchar();

cold_start:
    /* clear screen, set up terminal */
    vga_init(vgamode, vgafont);

    /* set heap at the start of Extended Memory (>1MiB), 1MiB in size */
    alloc_init((void*)0x00100000, (void*)0x001fffff);

warm_start:
    vga_clear();
    kprintf("%s\n%s", TXT_HELLO, TXT_MENU);
    kprintf("\nCurrent config: %dx%d size, %d mines, text mode %2Xh, graphic mode %2Xh\n", size, size, mines, vgamode, vgagmode);

    while (1) {
        char sel = getchar();
        
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
                vga_clear();
                kprintf(TXT_TEXT_MODES);
                getsn(ibuf, 256);
                vgamode = strtol(ibuf, NULL, 16);
                goto cold_start;
            } break;
            case 'f': {
                vga_clear();
                kprintf(TXT_FONT);
                getsn(ibuf, 256);
                vgafont = strtol(ibuf, NULL, 16);
                goto cold_start;
            } break;
            case 'g': {
                vga_clear();
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
