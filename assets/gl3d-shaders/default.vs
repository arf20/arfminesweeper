#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

uniform mat4 pm, vm, mm;

uniform vec3 color;
uniform vec3 light_dir;

out vec3 fcolor;


vec3 diffuse(vec3 sco_norm, vec3 l) {
    return color * max(dot(l, sco_norm), 0);
}

void main() {
    vec4 sco_norm = normalize(inverse(transpose(vm * mm)) * vec4(normal, 1.0));

    gl_Position = pm * vm * mm * vec4(vertex, 1.0);
    fcolor = (0.2*color) + diffuse(vec3(sco_norm), light_dir);
}

