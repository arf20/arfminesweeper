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

#ifndef _FRONTCONF_H
#define _FRONTCONF_H

#define FLAG_PNG_PATH       "../assets/flag.png"
#define FLAG_BMP_PATH       "../assets/flag.bmp"
#define FLAG_XBM_PATH       "../assets/flag.xbm"
#define FLAG_ICO_PATH       "../assets/flag.ico"
#define FONT_TTF_PATH       "../assets/FSEX300.ttf"
#define FONT_BMP_PATH       "../assets/charstrip.bmp"
#define GL3_VS_PATH         "../assets/msboard.vs"
#define GL3_FS_PATH         "../assets/msboard.fs"
#define JAVA_CLASS_PATH     "../assets/"
#define MODULE_SEARCH_PATH  "main_src/modules"

#define HEADER_HEIGHT 60
#define CELL_SIZE     20
#define CELL_MARGIN   2
#define W_MARGIN      5

#define TXT_TITLE "arfminesweeper"

#define TXT_LOST "\nYou died in a terrible explosion\n" \
    "Killed from a german S-Mine in Denmark\n" \
    "Your spontaneously dissasembled remains won't be recovered\n" \
    "and you have no family left to cry for you,\n" \
    "as they all died in the war.\n" \
    "Among you, there were 2600 german POWs employed by the danish\n" \
    "to clear mines between 1945 and 1947. As many as 500 died.\n" \
    "The game is now over.\n"

#define TXT_WON "\nYou have finished your task.\n" \
    "You have accopished the objective of clearing the 1.4M mines\n" \
    "laid off by the germans during the occupation of Denmark.\n" \
    "You won't be rewarded for your actions, and you will still be a POW\n" \
    "until 1949.\n" \
    "You have no family to return to, they all died in the war,\n" \
    "There is no good ending in a war.\n" \
    "But the game is now over.\n"

#define TXT_QUIT "\n\nYou tried sneaking out of the POW camp, how clever of you\n" \
    "You were caught 5km away from it, and shot in situ.\n"

#endif /* _FRONTCONF_H */

