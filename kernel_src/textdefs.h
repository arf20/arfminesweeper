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
    "\n"\
    "    [h] Halt CPU\n"\
    "    [s] Set board size\n"\
    "    [m] Set number of mines\n"\
    "    [v] Set VGA video mode\n"\
    "    [f] Set VGA video font\n"

#define TXT_VIDEO \
    "VGA modes (int 10h AH=00h):\n"\
    "    AL=00h 40x25, 320x200, 8x8\n"\
    "    AL=01h 40x25, 320x200, 8x8\n"\
    "    AL=02h 80x25, 720x400, 9x16\n"\
    "    AL=03h 80x25, 720x400, 9x16\n"\
    "    AL=07h 80x25, 720x350, 9x14\n"\
    "    AL=17h 80x43, 640x320, 8x8\n"\
    "    AL=58h 80x43, 640x348, 8x8\n"\
    "    AL=66h 80x50, 640x400, 8x8\n"\
    "AL> "

#define TXT_FONT \
    "VGA font (int 10h AH=11h):\n"\
    "    AL=04h,14h LOAD ROM 8x16 CHARACTER SET (VGA)\n"\
    "    AL=02h,12h LOAD ROM 8x8 DBL-DOT PATTERNS VGA\n"\
    "    AL=01h,11h MONOCHROME PATTERNS ROM VGA\n"\
    "AL> "
    

#endif /* _TEXTDEFS_H */
