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

    gl33.c: OpenGL 3.3/GLFW/GLEW frontend

*/

#include "glutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

GLint
shader_new(const char *vsp, const char *fsp) {
    /* 1. read vertex/fragment source */
    char *vsc, *fsc;
    FILE *vsf, *fsf;
    size_t vss, fss;
    GLint vs, fs, pid;

    vsf = fopen(vsp, "r");
    fsf = fopen(fsp, "r");

    if (vsf == NULL || fsf == NULL) {
        printf("Error reading shader: %s\n", strerror(errno));
        return 0;
    }

    fseek(vsf, 0L, SEEK_END);
    fseek(fsf, 0L, SEEK_END);

    vss = ftell(vsf);
    fss = ftell(fsf);

    vsc = malloc(vss + 1);
    fsc = malloc(fss + 1);

    fseek(vsf, 0L, SEEK_SET);
    fseek(fsf, 0L, SEEK_SET);

    fread(vsc, 1, vss, vsf);
    fread(fsc, 1, fss, fsf);

    vsc[vss] = 0;
    fsc[fss] = 0;

    /* 2. Compile shaders */
    unsigned int vertex, fragment;
    int success;
    char infoLog[1024];
    
    /* Compile vertex shader */
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char* const*)&vsc, NULL);
    glCompileShader(vs);
    
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 1024, NULL, infoLog);
        printf("Error compiling vertex shader: %s\n", infoLog);
        return 0;
    }
    
    /* Compile flagment shader */
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const char* const*)&fsc, NULL);
    glCompileShader(fs);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 1024, NULL, infoLog);
        printf("Error compiling fragment shader: %s\n", infoLog);
        return 0;
    }
    
    /* Shader Program */
    pid = glCreateProgram();
    glAttachShader(pid, vs);
    glAttachShader(pid, fs);
    glLinkProgram(pid);

    glGetProgramiv(pid, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pid, 1024, NULL, infoLog);
        printf("Error linking program: %s\n", infoLog);
        return 0;
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);

    return pid;
}

