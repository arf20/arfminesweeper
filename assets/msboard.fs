#version 330 core

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform vec2 sSize;
uniform uint size;
uniform uint board[1024];

out vec4 color;

#define BOARDXY(x, y)  board[((y) * size) + (x)]

/* Cell bit field */
#define CELL_BIT_MINE       0u
#define CELL_BIT_FLAG       1u
#define CELL_BIT_CLEAR      2u

/* Bit field check macros */
#define CHECK_MINE(x)       (((x) >> CELL_BIT_MINE) & 1u) > 0u
#define CHECK_FLAG(x)       (((x) >> CELL_BIT_FLAG) & 1u) > 0u
#define CHECK_CLEAR(x)      (((x) >> CELL_BIT_CLEAR) & 1u) > 0u

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



void main() {
    uint x = uint((gl_FragCoord.x - W_MARGIN) / (CELL_SIZE + CELL_MARGIN));
    uint y = uint((gl_FragCoord.y - HEADER_HEIGHT) / (CELL_SIZE + CELL_MARGIN));
    


    if (   gl_FragCoord.x > W_MARGIN
        && gl_FragCoord.x < sSize.x - W_MARGIN
        && gl_FragCoord.y > HEADER_HEIGHT
        && gl_FragCoord.y <sSize.y - W_MARGIN
        )
    {
        float cX = W_MARGIN + (float(x) * (CELL_SIZE + CELL_MARGIN));
        float cY = HEADER_HEIGHT + (float(y) * (CELL_SIZE + CELL_MARGIN));

        if (   gl_FragCoord.x > cX
            && gl_FragCoord.x < cX + CELL_SIZE
            && gl_FragCoord.y > cY
            && gl_FragCoord.y < cY + CELL_SIZE)
        {
            /* If clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {

            }
            /* Otherwise just a tile */
            else {
                color = vec4(C_WHITE, 1.0);
            }
        } else {
            color = vec4(C_BLACK, 1.0);
        }
    } else {
        color = vec4(C_BLACK, 1.0);
    } 
}
