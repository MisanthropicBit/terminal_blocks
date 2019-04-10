#ifndef TB_GAME_H
#define TB_GAME_H

#include "block.h"

// The structure used for the game's grid
typedef struct {
    int y;
    int x;
    int width;
    int height;
    tb_block_type* grid;
} tb_game_grid;

// Spawn a new, randomly rotated block
tb_block* tb_spawn_block(int y, int x);

// Runs the main game loop. Returns the next state to
// switch to, or zero if no such state
int tb_run_game(int* const final_score);

#endif // TB_GAME_H
