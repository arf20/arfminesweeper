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

*/

#ifndef _GLUTIL3D_H
#define _GLUTIL3D_H

#include <common/glad/include/glad/glad.h>

#include <cglm/cglm.h>

typedef struct {
    GLuint  vao;
    size_t  n;
    vec3    scale, pos;
                        GLuint  loc_mm;
    vec3    color;      GLuint loc_color;
    GLint   shader;
    GLenum  mode;
} model_t;

model_t *model_new(vec3 *verts, size_t n, vec3 scale, vec3 pos, vec3 color,
    GLint shader, GLenum mode);
model_t *model_draw(model_t *m);

#endif /* _GLUTIL3D_H */

