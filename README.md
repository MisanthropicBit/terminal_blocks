# Terminal Blocks ![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/MisanthropicBit/terminal_blocks.svg)

A small [1341 sloc](https://github.com/AlDanial/cloc) (as of
[7daa3b](https://github.com/MisanthropicBit/terminal_blocks/tree/7daa3b236390c85dcee6ac840a5649dfa9fc9b98))
line Tetris clone that can be played in the terminal. Requires
[`ncurses`](https://en.wikipedia.org/wiki/Ncurses).

```bash
> cloc include/ src/
      19 text files.
      19 unique files.
       0 files ignored.

github.com/AlDanial/cloc v 1.80  T=0.12 s (159.6 files/s, 15912.0 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                                9            283            134           1088
C/C++ Header                    10             77             59            253
-------------------------------------------------------------------------------
SUM:                            19            360            193           1341
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
* **w**: Move menu cursor up
* **s**: Move block/menu cursor down
* **j**: Rotate left
* **k**: Rotate right
* **p**: Play game (in menu) or pause/unpause game
* **g**: Toggle guides for where the focus block will land
* **f**: Confirm menu selection or drop block instantly to bottom
* **q**: Quit menu or game
