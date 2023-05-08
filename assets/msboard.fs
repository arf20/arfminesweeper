#version 330 core

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform vec2 sSize;

out vec4 color;

#define HEADER_HEIGHT 60.0
#define CELL_SIZE     20.0
#define CELL_MARGIN   2.0
#define W_MARGIN      5.0

#define C_WHITE  1.0, 1.0, 1.0
#define C_BLACK  0.0, 0.0, 0.0
#define C_YELLOW 1.0, 1.0, 0.0
#define C_RED    1.0, 0.0, 0.0
#define C_GREEN  0.0, 1.0, 0.0
#define C_BLUE   0.0, 0.0, 1.0
#define C_DBLUE  0.0, 0.0, 0.54
#define C_DRED   0.54, 0.0, 0.0
#define C_DCYAN  0.0, 0.54, 0.54
#define C_DGREY  0.66, 0.66, 0.66


float getX() {
    return gl_FragCoord.x;
}

float getY() {
    return gl_FragCoord.x;
}

void main() {
    int x = int((sSize.x - W_MARGIN) / (CELL_SIZE + CELL_MARGIN));
    int y = int((sSize.x - W_MARGIN) / (CELL_SIZE + CELL_MARGIN));

    if (   gl_FragCoord.x > W_MARGIN
        && gl_FragCoord.x < sSize.x - W_MARGIN
        && gl_FragCoord.y > HEADER_HEIGHT
        && gl_FragCoord.y < sSize.y - W_MARGIN
        )
    {
        color = vec4(C_WHITE, 1.0);
    } else {
        color = vec4(C_BLACK, 1.0);
    }
}
