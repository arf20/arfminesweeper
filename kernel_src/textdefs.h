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

*/

#ifndef _TEXTDEFS_H
#define _TEXTDEFS_H

#define TXT_HELLO \
    "arfminesweeper by arf20\n"\
    "Copyright 2023 Angel Ruiz Fernandez\n"\
    "License GPLv3+ <http://gnu.org/licenses/gpl.html>\n"

#define TXT_MENU \
    "Available frontends:\n"\
    "    [1] vgacon: VGA text mode console\n"\
    "    [2] vgatxt: VGA text mode\n"\
    "    [3] vgagra: VGA graphics mode\n"\
    "\n"\
    "    [h] Halt CPU\n"\
    "    [s] Set board size\n"\
    "    [m] Set number of mines\n"\
    "    [v] Set VGA text mode\n"\
    "    [f] Set VGA text font\n"\
    "    [g] Set VGA graphic mode\n"

#define TXT_TEXT_MODES \
    "VGA text modes (int 10h AH=00h):\n"\
    "    AL=00h 40x25, 320x200, 8x8,  16  greys\n"\
    "    AL=01h 40x25, 320x200, 8x8,  16  colors\n"\
    "    AL=02h 80x25, 720x400, 9x16, 16  greys\n"\
    "    AL=03h 80x25, 720x400, 9x16, 16  colors\n"\
    "    AL=07h 80x25, 720x350, 9x14, monochrome [MDA]\n"\
    "    AL=17h 80x43, 640x320, 8x8,  \n"\
    "    AL=58h 80x43, 640x348, 8x8,  \n"\
    "    AL=66h 80x50, 640x400, 8x8,  \n"\
    "AL> "

#define TXT_FONT \
    "VGA font (int 10h AH=11h):\n"\
    "    AL=04h,14h LOAD ROM 8x16 CHARACTER SET (VGA)\n"\
    "    AL=02h,12h LOAD ROM 8x8 DBL-DOT PATTERNS VGA\n"\
    "    AL=01h,11h MONOCHROME PATTERNS ROM VGA\n"\
    "AL> "

#define TXT_GRAPHIC_MODES \
    "VGA graphics modes (int 10h, AH=00h):\n"\
    "   AL=04h 320x200, 4   colors [CGA]\n"\
    "   AL=05h 320x200, 4   greys  [CGA]\n"\
    "   AL=06h 320x200, monochrome [CGA]\n"\
    "   AL=0Dh 320x200, 16  colors [EGA]\n"\
    "   AL=0Eh 640x200, 16  colors [EGA]\n"\
    "   AL=0Fh 640x350, monochrome [VGA]\n"\
    "   AL=10h 640x350, 16  colors [EGA]\n"\
    "   AL=11h 640x480, monochrome [VGA]\n"\
    "   AL=12h 640x480, 16  colors [VGA]\n"\
    "   AL=13h 320x200, 256 colors [VGA]\n"

#endif /* _TEXTDEFS_H */
