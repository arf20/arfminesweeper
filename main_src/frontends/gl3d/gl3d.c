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

    gl3d.c: OpenGL 3.3/GLFW/GLAD frontend

*/

#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include <common/glutil.h>

#include <common/frontconf.h>
#include "gl3d.h"
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

static const int *board = NULL;
static int size = 0;

static int wWidth, wHeight;
static GLFWwindow *window = NULL;

static mat4 camera = { 0 };

static void
update_camera() {
    glViewport(0, 0, wWidth, wHeight);

    vec3 obs = { 0.0f, 10.0f, -10.0f };
    vec3 vrp = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, 1.0f, 0.0f };
    mat4 vm, pm;
    glm_lookat(obs, vrp, up, vm);
    glm_perspective(M_PI * 90.0f / 180.0f, wWidth / wHeight, 0.1f, 50.0f, pm);

    glm_mat4_mul(pm, vm, camera);
}

static void
render(GLFWwindow *window) {
    glClearColor(C_BLACK, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Execute shader */
    //glUseProgram(boardShader);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void
resizeCallback(GLFWwindow *window, int w, int h) {
    wWidth = w;
    wHeight = h;
    update_camera();
}

static void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

static void
mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action != GLFW_RELEASE) return;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    int ix = 0, iy = 0;

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT: {
            gameClearCell(ix, iy);
        } break;
        case GLFW_MOUSE_BUTTON_RIGHT: {
            gameFlagCell(ix, iy);
        }
    }

}

int
gl3d_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = 1280; wHeight = 720;

    /* Init glfw context */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create window */
    if ((window = glfwCreateWindow(wWidth, wHeight, TXT_TITLE, NULL, NULL))
        == NULL)
    {
        printf("Error creating window\n");
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* Init glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Error initializing GLAD\n");
        return -1;
    }

    /* Set callbacks */
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSwapInterval(0);

    /* enable stuff */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    /* Compile shaders */
    GLint default_shader = program_new(GL3D_SHADER_PATH "default.vs", NULL,
        GL3D_SHADER_PATH "default.fs");
    if (default_shader < 0) {
        return -1;
    }

    update_camera();


    /* Enter the infinite event-processing loop */
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void
gl3d_destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

const char *
gl3d_name() {
    return "gl3d";
}

