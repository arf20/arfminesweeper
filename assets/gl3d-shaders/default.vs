#version 330 core

in vec3 vertex;

uniform mat4 camera;
uniform mat4 tg;

void main()
{
    gl_Position = camera * tg * vec4(vertex, 1.0);
}

