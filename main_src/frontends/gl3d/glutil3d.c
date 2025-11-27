/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This shader is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This shader is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this shader.  If not, see <https://www.gnu.org/licenses/>.

    glutil3d.c: OpenGL 3D utils

*/

#include "glutil3d.h"

#include <stdlib.h>
#include <string.h>

static void
compute_normals(vec3 *verts, vec3 *normals, size_t n) {
    vec3 u, v, normal;
    for (size_t i = 0; i < n / 3; i += 3) {
        glm_vec3_sub(verts[i + 1], verts[i + 0], u);
        glm_vec3_sub(verts[i + 2], verts[i + 0], v);
        glm_vec3_crossn(u, normal, normal);
        glm_vec3_copy(normal, normals[i + 0]);
        glm_vec3_copy(normal, normals[i + 1]);
        glm_vec3_copy(normal, normals[i + 2]);
    }
}

model_t *
model_new(vec3 *verts, size_t n, mat4 mm, vec3 color, GLint shader) {
    model_t *m = malloc(sizeof(model_t));

    m->n = n;

    glGenVertexArrays(1, &m->vao);
    glBindVertexArray(m->vao);

    GLuint vbo[2];
    glGenBuffers(2, vbo);

    /* vertices */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * n, verts,
        GL_STATIC_DRAW);
    
    GLuint loc_vertex = glGetAttribLocation(shader, "vertex");
    glVertexAttribPointer(loc_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(loc_vertex);
    
    /* normals */
    vec3 *normals = malloc(sizeof(vec3) * n);
    compute_normals(verts, normals, n);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * n, normals,
        GL_STATIC_DRAW);

    GLuint loc_normal = glGetAttribLocation(shader, "normal");
    glVertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(loc_normal);

    free(normals);

    glm_mat4_copy(mm, m->mm);
    m->loc_mm = glGetUniformLocation(shader, "mm");

    glm_vec3_copy(color, m->color);
    m->loc_color = glGetUniformLocation(shader, "color");

    m->shader = shader;

    return m;
}

model_t *
model_draw(const model_t *m) {
    glUniformMatrix4fv(m->loc_mm, 1, GL_FALSE, (float*)m->mm);
    glUniform3fv(m->loc_color, 1, m->color);

    glBindVertexArray(m->vao);
    glDrawArrays(GL_TRIANGLES, 0, m->n);
    glBindVertexArray(0);
}

