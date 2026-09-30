

module minesweeper (
    input logic ix [7:0],
    input logic iy [7:0],

    input logic set_size,
    input logic set_mine,
    input logic set_flag,
    input logic clear,

    input logic reset,
    input logic clk,

    output logic is_clear,
    output logic num [3:0],

    output logic game_won,
    output logic game_lost,

    output logic ready
);

    logic [7:0] x = ix + 1;
    logic [7:0] y = iy + 1;

    bit [2:0] board[0:127][0:127];
    bit [7:0] size = 7'd10;

    always_ff @(postedge clk) begin
        if (!game_won && !game_lost && ix < 126 && iy < 126) begin
            if (set_mine && !set_flag && !clear)
                board[x][y][0] = 1'b1;
            if (!set_mine && set_flag && !clear)
                if (!board[x][y][2]) board[x][y][1] = 1'b1;
            if (!set_mine && !set_flag && clear) begin
                if (!board[x][y][2]) begin
                    board[x][y][1] = 1'b1;
                    if (board[x][y][0])
                        game_lost = 1'b1;
                end
            end
        end
    end

    always_comb begin
        is_clear = board[x][y][2];

        num =
            board[x-1][y-1][0] +
            board[x  ][y-1][0] +
            board[x+1][y-1][0] +
            board[x-1][y  ][0] +
            board[x+1][y  ][0] +
            board[x-1][y+1][0] +
            board[x  ][y+1][0] +
            board[x+1][y+1][0];
    end


endmodule


module pe (
    input logic 
);




endmodule

