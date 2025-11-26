#version 330 core

in vec3 vertex;

uniform mat4 camera;
uniform mat4 mm;

void main()
{
    gl_Position = camera * mm * vec4(vertex, 1.0);
}

