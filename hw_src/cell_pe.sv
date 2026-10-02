module cell_pe (
    // PE bus
    //   control
    input logic pe_select,
    input logic pe_write_enable,
    //   inputs
    input logic set_mine,
    input logic set_flag,
    input logic reset_flag,
    input logic set_clear,
    //   outputs
    output logic flag,

    // PE interconnects
    input logic [7:0] surr_mine,
    input logic [7:0] surr_clear,

    // PE
    //   inputs
    input logic reset,
    input logic valid,
    input logic clk,
    //   outputs
    output logic mine,
    output logic clear,
    output logic won,
    output logic lost,
    output logic ready
);

    logic mined;
    logic flagged;
    logic cleared;
    logic new_state;

    always_ff @(posedge clk) begin
        if (reset) mined <= flagged <= cleared <= 1'b0;

        if (pe_select & pe_write_enable) begin
            if (set_mine) mined <= 1'b1;

            if (set_flag) flagged <= 1'b1;
            else if (reset_flag) flagged <= 1'b0;

            if (set_clear) cleared <= 1'b1;
        end

        if (valid) begin
            new_state <= ~mined & ~&surr_mine & &surr_clear;
            ready <= new_state == cleared;
            cleared <= cleared | new_state;
        end
    end

    always_comb begin
        // PE bus
        if (pe_select) begin
            mine = mined;
            flag = flagged;
            clear = cleared;
        end else begin
            mine = 1'bx;
            flag = 1'bx;
            clear = 1'bx;
        end

        won = (mined & flagged) | (~mined & cleared);
        lost = mined & cleared;
    end

endmodule

