#version 330 core

uniform int size, sWidth, sHeight;

layout (points, max_vertices = 1000) out;
out vec3 vColor;


#define HEADER_HEIGHT 60
#define CELL_SIZE     20
#define CELL_MARGIN   2
#define W_MARGIN      5

void glVertex(int x, int y) {
    gl_Position.x = -1.0 + (2.0 * ((float)x / (float)wWidth));
    gl_Position.y = -1.0 + (2.0 * ((float)y / (float)sHeight));
    gl_Position.z = 0;
    EmitVertex();
}

void main() {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int cx = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cy = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));

            glVertex(cx, cy);
            glVertex(cx, cy + CELL_SIZE);
            glVertex(cx + CELL_SIZE, cy + CELL_SIZE);
            glVertex(cx + CELL_SIZE, cy);
        }
    }
}
