#include <iostream>
#include <sstream>

#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Vminesweeper.h"

int inputPrompt(Vminesweeper *ms) {
    std::string line, cmd, sval;
    int val;
    while (true) {
        printf(" x <= %d\n y <= %d\n we <= %d\n set_mine <= %d\n"
            " set_flag <= %d\n reset_flag <= %d\n set_clear <= %d\n set_size <= %d\n"
            " reset <= %d\n valid <= %d\n",
            ms->x,
            ms->y,
            ms->write_enable,
            ms->set_mine,
            ms->set_flag,
            ms->reset_flag,
            ms->set_clear,
            ms->set_size,
            ms->reset,
            ms->valid);

        printf("hms> ");
        std::getline(std::cin, line);

        std::stringstream ss(line);
        std::getline(ss, cmd, ' ');
        std::getline(ss, sval, ' ');

        if (cmd == "step")
            return 0;
        else if (cmd == "exit")
            return -1;
        
        val = std::atoi(sval.c_str());

        if (cmd == "x")
            ms->x = val;
        if (cmd == "y")
            ms->y = val;
        else if (cmd == "we")
            ms->write_enable = val;
        else if (cmd == "set_mine")
            ms->set_mine = val;
        else if (cmd == "set_flag")
            ms->set_flag = val;
        else if (cmd == "reset_flag")
            ms->reset_flag = val;
        else if (cmd == "set_clear")
            ms->set_clear = val;
        else if (cmd == "set_size")
            ms->set_size = val;
        else if (cmd == "reset")
            ms->reset = val;
        else if (cmd == "valid")
            ms->valid = val;
    }
}

int main(int argc, char **argv, char **env) {
    Vminesweeper *ms = new Vminesweeper;
    vluint64_t sim_time = 0;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    ms->trace(m_trace, 5);
    m_trace->open("waveform.vcd");

    while (true) {
        if (inputPrompt(ms))
            break;

        // cycle
        ms->clk ^= 1;
        ms->eval();
        m_trace->dump(sim_time);
        sim_time++;
        ms->clk ^= 1;
        ms->eval();
        m_trace->dump(sim_time);
        sim_time++;

        printf(" mine >= %d\n flag >= %d\n clear >= %d\n num >= %d\n won >= %d\n"
            " lost >= %d\n ready >= %d\n\n\n",
            ms->mine,
            ms->flag,
            ms->clear,
            ms->num,
            ms->game_won,
            ms->game_lost,
            ms->ready);

    }

    m_trace->close();
    delete ms;
}

