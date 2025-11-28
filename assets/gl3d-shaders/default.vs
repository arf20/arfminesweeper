#version 330 core

layout (location = 0) in vec3 vertex;

uniform mat4 pm, vm, mm;

uniform vec3 color;

out vec3 fcolor;


void main() {
    gl_Position = pm * vm * mm * vec4(vertex, 1.0);
    fcolor = color;
}

