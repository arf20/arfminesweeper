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

    opengl_1_1_glut.c: OpenGL 1.1/FreeGLUT frontend

*/

#include <GL/freeglut.h>

#include "common.h"
#include "opengl_1_1_glut.h"
#include "../game.h"

static const int *board = NULL;
static int size = 0;

static int w = 0;

static void
render() {

}

static void
reshape(GLsizei l_width, GLsizei l_height) {

}

static void
keyboard(unsigned char key, int x, int y) {

}

int
GL11Start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    /* Init FreeGLUT */
    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);
    /* MSAA */
    glutSetOption(GLUT_MULTISAMPLE, 4);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    
    glutInitContextVersion(1, 1);
    /* Create Window */
    glutInitWindowSize(640, 480);
    w = glutCreateWindow(TXT_TITLE);

    /* Set callbacks */
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    /* MSAA config */
    GLint iMultiSample = 0;
    GLint iNumSamples = 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);

    /* Enter loop */
    glutMainLoop();
}

void
GL11Destroy() {
    glutDestroyWindow(w);
}
