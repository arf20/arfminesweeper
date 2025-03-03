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

    gl_ffp.c: OpenGL 1.2 FFP frontend

*/


#include <string.h>
#include <stdio.h>

#include <GL/gl.h>

#include "gl_ffp.h"

#include <common/frontconf.h>
#include <common/game.h>


#define C_WHITE  1.0f, 1.0f, 1.0f
#define C_BLACK  0.0f, 0.0f, 0.0f
#define C_YELLOW 1.0f, 1.0f, 0.0f
#define C_RED    1.0f, 0.0f, 0.0f
#define C_GREEN  0.0f, 1.0f, 0.0f
#define C_BLUE   0.0f, 0.0f, 1.0f
#define C_DBLUE  0.0f, 0.0f, 0.54f
#define C_DRED   0.54f, 0.0f, 0.0f
#define C_DCYAN  0.0f, 0.54f, 0.54f
#define C_DGREY  0.66f, 0.66f, 0.66f

#define TXT_OFFX 5
#define TXT_OFFY 1

static void
glTransformedVertex(int x, int y) {
    GLfloat tx = -1.0f + (2.0f * ((float)x / (float)wWidth));
    GLfloat ty = 1.0f - (2.0f * ((float)y / (float)wHeight));
    glVertex2f(tx, ty);
}

static void
drawString(int x, int y, const char *str, 
    GLfloat r, GLfloat g, GLfloat b)
{
    drawGLUTStringN(x, y, str, strlen(str), r, g, b);
}

static void
drawFlag(int x, int y) {
    glColor3f(C_RED);
    glBegin(GL_POLYGON);
    glTransformedVertex(x + (CELL_SIZE/4), y + 2);
    glTransformedVertex(x + (CELL_SIZE/4), y + (CELL_SIZE - 2));
    glTransformedVertex(x + ((CELL_SIZE/4)+2), y + (CELL_SIZE - 2));
    glTransformedVertex(x + ((CELL_SIZE/4)+2), y + (CELL_SIZE/2));
    glTransformedVertex(x + ((3*CELL_SIZE/4)+2), y + ((CELL_SIZE/2)-3));
    glEnd();
}

void
glRender() {
    static char buff[256];

    /* Clear screen */
    glClearColor(C_BLACK, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw title */
    drawString(5, 5, TXT_TITLE, C_WHITE);

    /* Check game state*/
    switch (gameGetState()) {
        case STATE_LOST: {
            drawGLUTTextMultiline(5, 45, TXT_LOST);
            return;
        } break;
        case STATE_WON: {
            drawGLUTTextMultiline(5, 45, TXT_WON);
            return;
        } break;
    }

    /* Print flags left */
    snprintf(buff, 256, "%d", gameGetFlagsLeft());
    drawString(wWidth - 25, 35, buff, C_WHITE);

    /* Render cell matrix */
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
                        case 1: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_BLUE); break;
                        case 2: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_GREEN); break;
                        case 3: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_RED); break;
                        case 4: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_DBLUE); break;
                        case 5: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_DRED); break;
                        case 6: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_DCYAN); break;
                        case 7: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_BLACK); break;
                        case 8: drawString(cX + TXT_OFFX, cY + TXT_OFFY, buff,
                            C_DGREY); break;
                    }
                    glColor3f(C_WHITE);
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                glColor3f(C_WHITE);
                glBegin(GL_QUADS);
                glTransformedVertex(cX, cY);
                glTransformedVertex(cX, cY + CELL_SIZE);
                glTransformedVertex(cX + CELL_SIZE, cY + CELL_SIZE);
                glTransformedVertex(cX + CELL_SIZE, cY);
                glEnd();
                drawFlag(cX, cY);
            }
            /* Otherwise just a tile */
            else {
                glColor3f(1.0f, 1.0f, 1.0f);
                glBegin(GL_QUADS);
                glTransformedVertex(cX, cY);
                glTransformedVertex(cX, cY + CELL_SIZE);
                glTransformedVertex(cX + CELL_SIZE, cY + CELL_SIZE);
                glTransformedVertex(cX + CELL_SIZE, cY);
                glEnd();
            }
        }
    }

    glEnd();
}
