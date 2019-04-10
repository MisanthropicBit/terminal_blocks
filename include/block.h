#ifndef TB_BLOCK_H
#define TB_BLOCK_H

#include "constants.h"

// Block types
typedef enum {
    // We set the enum to start set 1
    // so we can use 0 to signify an
    // empty position in the game grid
    TB_BLOCK_TYPE_L = 1,
    TB_BLOCK_TYPE_J,
    TB_BLOCK_TYPE_Z,
    TB_BLOCK_TYPE_S,
    TB_BLOCK_TYPE_T,
    TB_BLOCK_TYPE_I,
    TB_BLOCK_TYPE_BOX
} tb_block_type;

typedef enum {
    None = 0,
    Left,
    Right,
    Down
} tb_direction;

// Rotation types
typedef enum {
    Clockwise = 0,
    CounterClockwise
} tb_rotation;

// Simple block struct
typedef struct {
    // The coordinates of the square which contains the
    // gravitational center of the block about which we
    // rotate
    float cx;
    float cy;

    tb_block_type type;
    int rotation;
    int color;

    // All blocks consist of four squares
    // with two coordinates each
    int squares[TB_SQUARES_PER_BLOCK][2];
} tb_block;

/// Allocate a block
tb_block* tb_allocate_block(int y,
                            int x,
                            tb_block_type type,
                            int rotation,
                            int color);

/// Deallocate a block
void tb_deallocate_block(tb_block* block);

/// Set a block's position
void tb_set_block(tb_block* block, int y, int x);

/// Clear a block from the screen
void tb_clear_block(tb_block* block);

/// Move a block
void tb_move_block(tb_block* block, int dy, int dx);

/// Draw a block
void tb_draw_block(tb_block* block);

/// Rotate the squares of a block
void tb_rotate_squares(tb_block* block,
                       int dst_squares[4][2],
                       tb_rotation rotation);

#endif // TB_BLOCK_H
