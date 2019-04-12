# Terminal Blocks [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/MisanthropicBit/vim-analog/master/LICENSE)

A small [1232 sloc](https://github.com/AlDanial/cloc) (as of
[8119c1](https://github.com/MisanthropicBit/terminal_blocks/tree/8119c153360b3ac7dcd1477c9367ef49903d343d))
line Tetris clone that can be played in the terminal. Requires
[`ncurses`](https://en.wikipedia.org/wiki/Ncurses).

```bash
> cloc include/ src/
      19 text files.
      19 unique files.
       0 files ignored.

github.com/AlDanial/cloc v 1.80  T=0.20 s (93.5 files/s, 8608.4 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                                9            255            130            996
C/C++ Header                    10             76             57            236
-------------------------------------------------------------------------------
SUM:                            19            331            187           1232
-------------------------------------------------------------------------------
```

![The game in action](/game.gif)

## Installing

Clone the repository and run `make` in the game's top-level directory. This
creates the game executable `terminal_blocks`.

```bash
> git clone https://github.com/MisanthropicBit/terminal_blocks
> make
> ./terminal_blocks
```

To combat boredom, you can add the game's directory to your `PATH`.

```bash
PATH=$PATH:<path-to-terminal_blocks>
```

Or add an alias for the executable.

```bash
alias terminal_blocks="<path-to-terminal_blocks-executable>"
```

## Controls

* **a**: Move block left
* **d**: Move block right
* **s**: Move block down
* **j**: Rotate left
* **k**: Rotate right
* **p**: Play game (in menu) or pause/unpause game
* **f**: Confirm menu selection or drop block instantly to bottom
* **q**: Quit menu or game
