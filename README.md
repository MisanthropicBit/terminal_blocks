# Terminal Blocks [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/MisanthropicBit/vim-analog/master/LICENSE)

A small [1248 sloc](https://github.com/AlDanial/cloc) (as of sha256) line Tetris
clone that can be played in the terminal. Requires
[`ncurses`](https://en.wikipedia.org/wiki/Ncurses).

![The game in action](/game.gif)

```bash
> cloc include/ src/
      19 text files.
      19 unique files.
       0 files ignored.

github.com/AlDanial/cloc v 1.80  T=0.06 s (297.3 files/s, 27855.6 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                                9            259            140           1008
C/C++ Header                    10             76             57            240
-------------------------------------------------------------------------------
SUM:                            19            335            197           1248
-------------------------------------------------------------------------------

```

## Installing

Run `make` in the game's top-level directory. This creates the game executable
`terminal_blocks`.

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

## TODO

- [ ] Implement entering and saving highscores
- [ ] Fix collision detection (possibly off-by-one error)
- [ ] Register appropriate signal handlers
