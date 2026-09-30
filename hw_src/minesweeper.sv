module minesweeper (
    input logic x[7:0],
    input logic y[7:0],

    input logic set_size,
    input logic set_mine,
    input logic set_flag,
    input logic clear,

    input logic reset,
    input logic clk,

    output logic is_clear,
    output logic num[3:0],

    output logic game_won,
    output logic game_lost,

    output logic ready
);

    logic [7:0] size;

    always_ff @(posedge clk) begin

    end

    always_comb begin

    end

endmodule


module cell_pe (
    input logic set_mine,
    input logic set_flag,
    input logic set_clear,
    input logic reset,

    input logic surr_mine[7:0],
    input logic surr_clear[7:0],

    input logic valid,
    input logic clk,

    output logic mine,
    output logic flag,
    output logic clear,

    output logic won,
    output logic lost,

    output logic ready
);

    logic mined, flagged, cleared;

    always_ff (@posedge clk) begin
        if (valid) begin
            if (set_mine) mined <= 1'b1;
            if (set_flag) flagged <= 1'b1;
            if (reset) mined <= flagged <= cleared <= 1'b0;

            logic new_state <= set_clear | (~mined & ~&surr_mine & &surr_clear);
            ready <= new_state == cleared;
            cleared |= new_state;
        end
    end

    always_comb begin
        mine = mined;
        flag = flagged;
        clear = cleared;

        won = (mined & flagged) | (~mined & cleared);
        lost = mined & cleared;
    end

endmodule

