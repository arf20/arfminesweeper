module minesweeper #(
    parameter order = 4,
    cap = 2**order
)(
    // cell
    //   address
    input logic [order-1:0] x,
    input logic [order-1:0] y,
    //   control
    input logic write_enable,
    //   input
    input logic set_mine,
    input logic set_flag,
    input logic reset_flag,
    input logic set_clear,
    //   output
    output logic mine,
    output logic flag,
    output logic clear,
    output logic [3:0] num,

    // board
    //   input
    input logic set_size,
    input logic reset,
    
    input logic valid,
    input logic clk,

    //   output
    output logic game_won,
    output logic game_lost,

    output logic ready
);

    // registers
    logic [order-1:0] size;

    struct {
        logic won;
        logic lost;
    } game_state;   // mutually exclusive

    // PE bus select lines
    logic [cap-1:0] pe_ver_select;
    logic [cap-1:0] pe_hor_select;

    // PE outputs
    logic pe_mines[0:cap-1][0:cap-1];
    logic pe_clears[0:cap-1][0:cap-1];
    logic [cap-1:0][cap-1:0] pe_won;
    logic [cap-1:0][cap-1:0] pe_lost;
    logic [(cap*cap)-1:0] pe_ready;

    // PE addressed bus
    logic pe_bus_set_mine;
    logic pe_bus_set_flag;
    logic pe_bus_reset_flag;
    logic pe_bus_set_clear;
    logic pe_bus_flag;

    genvar gx, gy;
    generate
        for (gx = 0; gx < cap; gx++) begin : row
            for (gy = 0; gy < cap; gy++) begin : col
                cell_pe u_cell(
                    // PE bus control
                    .pe_select(pe_hor_select[gx] & pe_ver_select[gy]),
                    .pe_write_enable(write_enable),
                    // PE bus inputs
                    .set_mine(pe_bus_set_mine),
                    .set_flag(pe_bus_set_flag),
                    .reset_flag(pe_bus_reset_flag),
                    .set_clear(pe_bus_set_clear),
                    // PE bus outputs
                    .flag(pe_bus_flag),

                    // PE interconnects
                    //   mines
                    .surr_mine({
                        ((gx>1     && gy>1    ) ? pe_mines[gx-1][gy-1] : 1'b0),
                        ((            gy>1    ) ? pe_mines[gx  ][gy-1] : 1'b0),
                        ((gx<cap-1 && gy>1    ) ? pe_mines[gx-1][gy-1] : 1'b0),
                        ((gx>1                ) ? pe_mines[gx-1][gy  ] : 1'b0),
                        ((gx<cap-1            ) ? pe_mines[gx+1][gy  ] : 1'b0),
                        ((gx>1     && gy<cap-1) ? pe_mines[gx-1][gy+1] : 1'b0),
                        ((            gy<cap-1) ? pe_mines[gx  ][gy+1] : 1'b0),
                        ((gx<cap-1 && gy<cap-1) ? pe_mines[gx+1][gy+1] : 1'b0)
                    }),
                    //   clears
                    .surr_clear({
                        ((gx>1     && gy>1    ) ? pe_clears[gx-1][gy-1] : 1'b0),
                        ((            gy>1    ) ? pe_clears[gx  ][gy-1] : 1'b0),
                        ((gx<cap-1 && gy>1    ) ? pe_clears[gx-1][gy-1] : 1'b0),
                        ((gx>1                ) ? pe_clears[gx-1][gy  ] : 1'b0),
                        ((gx<cap-1            ) ? pe_clears[gx+1][gy  ] : 1'b0),
                        ((gx>1     && gy<cap-1) ? pe_clears[gx-1][gy+1] : 1'b0),
                        ((            gy<cap-1) ? pe_clears[gx  ][gy+1] : 1'b0),
                        ((gx<cap-1 && gy<cap-1) ? pe_clears[gx+1][gy+1] : 1'b0)
                    }),

                    // PE inputs
                    .reset(reset),
                    .valid(valid),
                    .clk(clk),
                    // PE outputs
                    .mine(pe_mines[gx][gy]),
                    .clear(pe_clears[gx][gy]),
                    .won(pe_won[gx][gy]),
                    .lost(pe_lost[gx][gy]),
                    .ready(pe_ready[(cap*gy)+gx])
                );
            end
        end
    endgenerate

    logic all_won;
    logic one_lost;

    always_ff @(posedge clk) begin
        if (set_size) size <= x;

        if (all_won)    game_state.won  <= 1'b1;
        if (one_lost)   game_state.lost <= 1'b1;

        if (reset) begin
            game_state.won <= 0;
            game_state.lost <= 0;
        end

        ready <= &pe_ready;
    end

    always_comb begin
        pe_hor_select = 1'b1 << x;
        pe_ver_select = 1'b1 << x;

        // cell
        pe_bus_set_mine = set_mine;
        pe_bus_set_flag = set_flag;
        pe_bus_reset_flag = reset_flag;
        pe_bus_set_clear = set_clear;

        mine = pe_mines[x][y];
        flag = pe_bus_flag;
        clear = pe_clears[x][y];
        // number function exists once
        num =
            ((x>1             && y>1            ) ?  4'(pe_mines[x-1][y-1]) : 4'b0) +
            ((                   y>1            ) ?  4'(pe_mines[x  ][y-1]) : 4'b0) +
            ((x<order'(cap-1) && y>1            ) ?  4'(pe_mines[x+1][y-1]) : 4'b0) +
            ((x>1                               ) ?  4'(pe_mines[x-1][y  ]) : 4'b0) +
            ((x<order'(cap-1)                   ) ?  4'(pe_mines[x+1][y  ]) : 4'b0) +
            ((x>1             && y<order'(cap-1)) ?  4'(pe_mines[x-1][y+1]) : 4'b0) +
            ((                   y<order'(cap-1)) ?  4'(pe_mines[x  ][y+1]) : 4'b0) +
            ((x<order'(cap-1) && y<order'(cap-1)) ?  4'(pe_mines[x+1][y+1]) : 4'b0);

        // submatrix reduction (expensive)
        all_won = 1'b1;
        for (int i = 0; i < size; i++) begin
            for (int j = 0; j < size; j++) begin
                all_won &= pe_won[i][j];
            end
        end
        // again
        one_lost = 1'b0;
        for (int i = 0; i < size; i++) begin
            for (int j = 0; j < size; j++) begin
                one_lost |= pe_lost[i][j];
            end
        end

        game_won  = game_state.won;
        game_lost = game_state.lost;
    end

endmodule

