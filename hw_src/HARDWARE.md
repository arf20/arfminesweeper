# Hardware target

Minesweeper acceleration implementation in hardware

The hardware will be either FPGA or a tiny tapeout ASIC.

It should support at least 100x100 square boards.

## Top-level interface specification

Parallel input and outputs, active high. Valid-ready protocol.

### Inputs

- Board control:
  - set_size:   X input value is set as board size
  - reset:      resets the board
- Cell control:
  - X,Y[4]:     7-bit cell address
  - set_mine:   sets mine bit (mx)
  - set_flag:   sets flag bit if not clear (mx)
  - set_clear:  clear mine (mx)
- clk:          constant clock
- valid:        input validity signal

(mx): should be mutually exclusive (not garanteed)

### Outputs

- Game output
  - game_won:   game won (mx)
  - game_lost:  game lost (mx)
- Cell outputs:
  - is_clear:   cell is clear
  - num[4]:     4-bit cell mine number
- ready:        output ready signal

(mx): mutually exclusive

### Computer interface

The parallel interface will connect with an MCU that will be responsible of
translating the parallel interface of the accelerator with apropiate valid-ready timing
with a serial UART to interface with the host client computer.

The UART serial interface will be text command based, as follows

#### `b<size><lf>`

resets board, sets size (decimal), follows a board character map specified below

##### Board character map

size lines size wide characters, lf terminated. XY 0,0 is top left corner.
character `.` is absence of mine, while `#` is presence of mine

Example:

```
........<lf>
.#..#...<lf>
......#.<lf>
......#.<lf>
..#.....<lf>
...#....<lf>
#....#..<lf>
#.......<lf>
```

Response: `ok<lf>`

#### `br<size>,<mines><lf>`

resets board, sets size (decimal) and generates a random board on the MCU with its seed
with the specified number of mines (decimal)

Response: `ok<lf>`

#### `c<x>,<y><lf>`

attempt to clear cell

Response: `ok<lf>` if no game state change where num is mine number and
follows a board character map specified below.
`won<lf>` or `lost<lf>` if game state changes

##### Clear character map

size lines size wide characters, lf terminated. XY 0,0 is top left corner.
character `#` is uncleared, while `.` is cleared cell with no surrounding mines
and a number designates the number of surrounding mines

Example:

```
3#####1.<lf>
######1.<lf>
#####21.<lf>
#####3..<lf>
#####21.<lf>
######11<lf>
#######1<lf>
2#####3#<lf>
```

#### `f<y>,<y><lf>`

flag uncleared cell

Response: `ok<lf>` or `cleared<lf>` if cell is already cleared

## Architecture specification

Systolic array of Processing Elements (PE) synthetised from the recursive clearing
function implemented in `game.c`.

Each processing elements corresponds with a cell. The PEs will each have the cell control
signals of set_mine, set_flag and set_signal. These are addressed with the XY cell address bus.
Being a ff block, they will implicitely also have a clock.

They will have a mine output and a clear output, directly connected to each of the 8 surrounding
mines.

So, with this information, each mine can compute if it should be clear according to its
surroundings each clock cycle with a simple function.

The process begins the moment valid goes high (immediately sets ready low),
using the state of the inputs, and the end is signaled when the systolic array
has propagated the function and has reached a stable state with the ready signal
going high. This is determined by having a bit bus across all the PEs that is set
whenever a cell is cleared.

