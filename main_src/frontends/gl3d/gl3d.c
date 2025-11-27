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
#include "glutil3d.h"
#include "models.h"

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

static GLuint default_shader = -1;
static GLuint loc_pm = -1, loc_vm = -1, loc_light_dir = -1;

static int orbit_az = 0, orbit_el = 0, orbit_d = 0;
static float az = M_PI / 4.0, el = M_PI / 4.0, d = -4;
static mat4 pm = { 0 }, vm = { 0 };
static vec3 light_dir = { 0 };

static model_t *model_cube = NULL;



static void
update_camera() {
    glViewport(0, 0, wWidth, wHeight);

    vec3 obs = { 0, 0, d };
    glm_vec3_rotate(obs, el, (vec3){1, 0, 0});
    glm_vec3_rotate(obs, az, (vec3){0, 1, 0});
    vec3 vrp = { 0, 0, 0 };
    vec3 up = { 0, 1, 0 };

    glm_lookat(obs, vrp, up, vm);
}

static void
init_camera() {
    glm_perspective(M_PI * 90.0f / 180.0f, (float)wWidth / (float)wHeight,
        0.1f, 50.0f, pm);
    update_camera();
}

static void
render(GLFWwindow *window) {
    glClearColor(C_BLACK, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    az += 0.0001f * orbit_az;
    el += 0.0001f * orbit_el;
    d  += 0.0002f * orbit_d;
    if (orbit_az || orbit_el || orbit_d)
        update_camera();

    glUseProgram(default_shader);
    glUniformMatrix4fv(loc_pm, 1, GL_FALSE, (float*)pm);
    glUniformMatrix4fv(loc_vm, 1, GL_FALSE, (float*)vm);
    model_draw(model_cube);
    
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
    /* orbit az */
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        orbit_az--;
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        orbit_az++;
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
        orbit_az++;
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        orbit_az--;
    /* orbit el */
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
        orbit_el--;
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        orbit_el++;
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
        orbit_el++;
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        orbit_el--;
    /* orbit el */
    else if (key == GLFW_KEY_E && action == GLFW_PRESS)
        orbit_d--;
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
        orbit_d++;
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        orbit_d++;
    else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        orbit_d--;
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
    default_shader = shader_new(GL3D_SHADER_PATH "default.vs", NULL,
        GL3D_SHADER_PATH "default.fs");
    if (default_shader < 0) {
        return -1;
    }
    loc_pm = glGetUniformLocation(default_shader, "pm");
    loc_vm = glGetUniformLocation(default_shader, "vm");
    loc_light_dir = glGetUniformLocation(default_shader, "light_dir");

    init_camera();
    glm_vec3_normalize_to((vec3){-1, -1, 0}, light_dir);

    model_cube = model_new(cube, cube_size / sizeof(vec3),
        (mat4)GLM_MAT4_IDENTITY_INIT, (vec3){1.0f, 1.0f, 1.0f}, default_shader);


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

