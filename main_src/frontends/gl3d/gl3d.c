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

#define CELL_PITCH  1.1

#define BASE_COLOR      { 0.75, 0.75, 0.75 }
#define CELL_COLOR      { 1, 1, 1 }
#define CURSOR_COLOR    { 0.2, 0.2, 1 }
#define FLAG_COLOR      { 1, 0, 0 }

static const int *board = NULL;
static int size = 0;

static int wWidth, wHeight;
static GLFWwindow *window = NULL;

static GLuint default_shader = -1, shaded_shader = -1;
static GLuint default_loc_pm = -1, default_loc_vm = -1, 
    shaded_loc_pm = -1, shaded_loc_vm = -1, shaded_loc_light_dir = -1;

static int orbit_az = 0, orbit_el = 0, orbit_d = 0;
static float az = 0/* M_PI / 4.0*/, el = M_PI / 4.0, d = 10;
static mat4 pm = { 0 }, vm = { 0 };
static vec3 light_dir = { 0 };

static model_t *base = NULL, *cell = NULL, *cursor = NULL, *m_flag = NULL;

static int curx = 0, cury = 0;


static void
update_camera() {
    glViewport(0, 0, wWidth, wHeight);

    vec3 obs = { 0, 0, d };
    glm_vec3_rotate(obs, -el, (vec3){1, 0, 0});
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
    static float last_time = 0.0f;
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    glClearColor(C_BLACK, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    az += 1.f * delta_time * orbit_az;
    el += 1.f * delta_time * orbit_el;
    d  += 2.f * delta_time * orbit_d;
    if (orbit_az || orbit_el || orbit_d)
        update_camera();

    glUseProgram(shaded_shader);
    /* camera */
    glUniformMatrix4fv(shaded_loc_pm, 1, GL_FALSE, (float*)pm);
    glUniformMatrix4fv(shaded_loc_vm, 1, GL_FALSE, (float*)vm);

    /* base */
    model_draw(base);

    glm_vec3_copy((vec3)CELL_COLOR, cell->color);
    /* cells */
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            float cX = (CELL_PITCH * (float)x) - (CELL_PITCH*(float)(size - 1)/2.0);
            float cZ = (CELL_PITCH * (float)y) - (CELL_PITCH*(float)(size - 1)/2.0);

            glm_vec3_copy((vec3){cX, 0, cZ}, cell->pos);

            /* If clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                glm_vec3_copy((vec3){cX, 1, cZ}, m_flag->pos);
                model_draw(m_flag);
                model_draw(cell);
            }
            /* Otherwise just a tile */
            else {
                model_draw(cell);
            }
        }
    }

    glUseProgram(default_shader);
    glUniformMatrix4fv(default_loc_pm, 1, GL_FALSE, (float*)pm);
    glUniformMatrix4fv(default_loc_vm, 1, GL_FALSE, (float*)vm);

    float cX = (CELL_PITCH * (float)curx) - (CELL_PITCH*(float)(size - 1)/2.0);
    float cZ = (CELL_PITCH * (float)cury) - (CELL_PITCH*(float)(size - 1)/2.0);
    glm_vec3_copy((vec3)CURSOR_COLOR, cursor->color);
    glm_vec3_copy((vec3){cX, 0, cZ}, cursor->pos);
    model_draw(cursor);
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

    /* cursor */
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        curx++;
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        curx--;
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        cury--;
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        cury++;

    else if (key == GLFW_KEY_C && action == GLFW_RELEASE)
        gameClearCell(curx, cury);
    else if (key == GLFW_KEY_F && action == GLFW_RELEASE)
        gameFlagCell(curx, cury);

    if (curx >= size)
        curx = 0;
    if (curx < 0)
        curx = size - 1;
    if (cury >= size)
        cury = 0;
    if (cury < 0)
        cury = size - 1;
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
    glEnable(GL_DEPTH_TEST);

    /* Compile shaders */
    default_shader = shader_new(GL3D_SHADER_PATH "default.vs", NULL,
        GL3D_SHADER_PATH "default.fs");
    if (shaded_shader < 0) {
        return -1;
    }
    default_loc_pm = glGetUniformLocation(default_shader, "pm");
    default_loc_vm = glGetUniformLocation(default_shader, "vm");
    
    shaded_shader = shader_new(GL3D_SHADER_PATH "shaded.vs", NULL,
        GL3D_SHADER_PATH "default.fs");
    if (shaded_shader < 0) {
        return -1;
    }
    
    shaded_loc_pm = glGetUniformLocation(shaded_shader, "pm");
    shaded_loc_vm = glGetUniformLocation(shaded_shader, "vm");
    shaded_loc_light_dir = glGetUniformLocation(shaded_shader, "light_dir");

    glUseProgram(shaded_shader);

    init_camera();
    glm_vec3_normalize_to((vec3){ 1,  1,  1}, light_dir);
    glUniform3fv(shaded_loc_light_dir, 1, light_dir);

    base = model_new(cube, cube_size / sizeof(vec3),
        (vec3){(float)size/2.0 + 1.0, 0.5, (float)size/2.0 + 1.0},
        (vec3){0, -1, 0},
        (vec3)BASE_COLOR, shaded_shader, GL_TRIANGLES);

    cell = model_new(cube, cube_size / sizeof(vec3),
        (vec3){0.5, 0.5, 0.5}, GLM_VEC3_ZERO,
        (vec3)CELL_COLOR, shaded_shader, GL_TRIANGLES);

    m_flag = model_new(flag, flag_size / sizeof(vec3),
        (vec3){0.5, 0.5, 0.5}, GLM_VEC3_ZERO,
        (vec3)FLAG_COLOR, shaded_shader, GL_TRIANGLES);

    cursor = model_new(wirecube, wirecube_size / sizeof(vec3),
        (vec3){0.55, 0.55, 0.55}, GLM_VEC3_ZERO,
        (vec3)CURSOR_COLOR, default_shader, GL_LINES);


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

