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

    gl11.c: FreeGLUT/OpenGL 1.2 FFP frontend

*/

#include <string.h>
#include <stdio.h>

#include <GL/freeglut.h>

#include "glut.h"
#include <common/frontconf.h>
#include <common/game.h>

#include "common/gl_ffp.h"

#define TXT_HEIGHT 15

const int *board = NULL;
int size = 0;

int wWidth, wHeight;
int window;

void
drawGLUTStringN(int x, int y, const char *str, int n,
    GLfloat r, GLfloat g, GLfloat b)
{
    GLfloat tx = -1.0f + (2.0f * ((float)x / (float)wWidth));
    GLfloat ty = 1.0f - (2.0f * (((float)y + (float)TXT_HEIGHT) / (float)wHeight));
    glColor3f(r, g, b);
    glRasterPos3f(tx, ty, 0.0f);
    for (int i = 0; i < n; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
}

void
drawGLUTTextMultiline(int x, int y, const char *str) {
    const char *line = str, *next = NULL;
    int len = strlen(str), i = 0;
    while (line < str + len) {
        next = strchr(line, '\n');
        drawGLUTStringN(x, y + (i * TXT_HEIGHT), line, next - line, 1.0f, 1.0f, 1.0f);
        line = next + 1;
        i++;
    }
}

static void
reshapeCallback(GLsizei l_width, GLsizei l_height) {

}

static void
keyboardCallback(unsigned char key, int x, int y) {

}

static void
mouseCallback(int button, int state, int x, int y) {
    if (state != GLUT_UP) return;

    int ix = (x - W_MARGIN) /
        (CELL_SIZE + CELL_MARGIN);
    int iy = (y - HEADER_HEIGHT) /
        (CELL_SIZE + CELL_MARGIN);
    if (ix < 0 || ix >= size || iy < 0 || iy >= size) return;

    switch (button) {
        case GLUT_LEFT_BUTTON: { /* Left */
            gameClearCell(ix, iy);
        } break;
        case GLUT_RIGHT_BUTTON: { /* Right */
            gameFlagCell(ix, iy);
        } break;
    }
}

static void
render() {
    glRender();
    glutSwapBuffers();
}

int
GLUTStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    /* Init FreeGLUT */
    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);
    /* MSAA */
    glutSetOption(GLUT_MULTISAMPLE, 4);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    
    glutInitContextVersion(1, 1);
    /* Create Window */
    glutInitWindowSize(wWidth, wHeight);
    window = glutCreateWindow(TXT_TITLE);

    /* Set callbacks */
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);

    /* MSAA config */
    GLint iMultiSample = 0;
    GLint iNumSamples = 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);

    /* Enter loop */
    glutMainLoop();
}

void
GLUTDestroy() {
    glutDestroyWindow(window);
}
