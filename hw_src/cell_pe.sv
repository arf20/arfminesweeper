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

    logic new_state_next;
    logic cleared_next;

    always_ff @(posedge clk) begin
        if (reset) mined <= flagged <= cleared <= 1'b0;

        if (pe_select & pe_write_enable) begin
            if (set_mine) mined <= 1'b1;

            if (set_flag & ~cleared) flagged <= 1'b1;
            else if (reset_flag) flagged <= 1'b0;

            if (set_clear & ~flagged) cleared <= 1'b1;
        end

        if (valid) begin
            cleared <= cleared_next;
        end
    end

    always_comb begin
        mine = mined;
        clear = cleared;

        // PE bus
        if (pe_select) begin
            flag = flagged;
        end else begin
            flag = 1'bx;
        end

        new_state_next = ~mined & ~&surr_mine & |surr_clear;
        cleared_next = cleared | new_state_next;
        ready = cleared_next == cleared;

        won = (mined & flagged) | (~mined & cleared);
        lost = mined & cleared;
    end

endmodule

