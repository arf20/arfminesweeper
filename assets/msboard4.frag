#version 460

layout(origin_upper_left) in vec4 gl_FragCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}