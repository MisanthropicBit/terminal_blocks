#include "block.h"
#include "terminal.h"
#include <ncurses.h>
#include <stdlib.h>

tb_block* tb_allocate_block(int y,
                            int x,
                            tb_block_type type,
                            int rotation,
                            int color) {
    tb_block* block = malloc(sizeof(tb_block));

    if (!block) {
        return NULL;
    }

    block->type = type;
    block->rotation = rotation; // TODO: Do the actual rotation?
    block->color = color;
    tb_set_block(block, y, x);

    return block;
}

void tb_deallocate_block(tb_block* block) {
    if (block) {
        free(block);
        block = NULL;
    }
}

void tb_set_block(tb_block* block, int y, int x) {
    if (!block) {
        return;
    }

    // Set the offsets of the squares based on the block's type. The
    // coordinate (x, y) denotes the top-left corner of the square which
    // contains the block's gravitational center
    switch (block->type) {
        case TB_BLOCK_TYPE_L:
            block->cy = y + 0.5f;
            block->cx = x + 0.5f;
            block->squares[0][0] = y - 1;
            block->squares[0][1] = x;
            block->squares[1][0] = y;
            block->squares[1][1] = x;
            block->squares[2][0] = y + 1;
            block->squares[2][1] = x;
            block->squares[3][0] = y + 1;
            block->squares[3][1] = x + 1;
            break;

        case TB_BLOCK_TYPE_J:
            block->cy = y + 0.5f;
            block->cx = x + 0.5f;
            block->squares[0][0] = y - 1;
            block->squares[0][1] = x;
            block->squares[1][0] = y;
            block->squares[1][1] = x;
            block->squares[2][0] = y + 1;
            block->squares[2][1] = x;
            block->squares[3][0] = y + 1;
            block->squares[3][1] = x - 1;
            break;

        case TB_BLOCK_TYPE_Z:
            block->cy = y + 1.0f;
            block->cx = x + 0.5f;
            block->squares[0][0] = y;
            block->squares[0][1] = x - 1;
            block->squares[1][0] = y;
            block->squares[1][1] = x;
            block->squares[2][0] = y + 1;
            block->squares[2][1] = x;
            block->squares[3][0] = y + 1;
            block->squares[3][1] = x + 1;
            break;

        case TB_BLOCK_TYPE_S:
            block->cy = y + 1.0f;
            block->cx = x + 0.5f;
            block->squares[0][0] = y;
            block->squares[0][1] = x + 1;
            block->squares[1][0] = y;
            block->squares[1][1] = x;
            block->squares[2][0] = y + 1;
            block->squares[2][1] = x;
            block->squares[3][0] = y + 1;
            block->squares[3][1] = x - 1;
            break;

        case TB_BLOCK_TYPE_T:
            block->cy = y + 0.5f;
            block->cx = x + 0.5f;
            block->squares[0][0] = y;
            block->squares[0][1] = x - 1;
            block->squares[1][0] = y;
            block->squares[1][1] = x;
            block->squares[2][0] = y;
            block->squares[2][1] = x + 1;
            block->squares[3][0] = y + 1;
            block->squares[3][1] = x;
            break;

        case TB_BLOCK_TYPE_I:
            block->cy = y + 1.0f;
            block->cx = x + 0.5f;
            block->squares[0][0] = y - 1;
            block->squares[0][1] = x;
            block->squares[1][0] = y;
            block->squares[1][1] = x;
            block->squares[2][0] = y + 1;
            block->squares[2][1] = x;
            block->squares[3][0] = y + 2;
            block->squares[3][1] = x;
            break;

        case TB_BLOCK_TYPE_BOX:
            block->cy = y + 1.0f;
            block->cx = x + 1.0f;
            block->squares[0][0] = y;
            block->squares[0][1] = x;
            block->squares[1][0] = y;
            block->squares[1][1] = x + 1;
            block->squares[2][0] = y + 1;
            block->squares[2][1] = x;
            block->squares[3][0] = y + 1;
            block->squares[3][1] = x + 1;
            break;

        default:
            break;
    }
}

void tb_clear_block(tb_block* block) {
    if (!block) {
        return;
    }

    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        mvaddch(block->squares[i][0], block->squares[i][1], ' ');
    }
}

void tb_move_block(tb_block* block,
                   int dy,
                   int dx) {
    if (!block) {
        return;
    }

    // Update the center of the block
    block->cy += dy;
    block->cx += dx;

    // Move the block's squares
    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        block->squares[i][0] += dy;
        block->squares[i][1] += dx;
    }
}

void tb_draw_block(tb_block* block) {
    if (!block) {
        return;
    }

    tb_set_color(block->color);
    attron(A_BOLD);

    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        mvaddch(block->squares[i][0],
                block->squares[i][1],
                TB_BLOCK_CHAR);
    }

    attroff(A_BOLD);
    tb_reset_color(block->color);
}

void tb_rotate_squares(tb_block* block,
                       int dst_squares[4][2],
                       tb_rotation rotation) {
    if (!block || !dst_squares) {
        return;
    }

    if (block->type == TB_BLOCK_TYPE_BOX) {
        // The box block is symmetry so it does not make sense to rotate
        return;
    }

    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        float ny = block->squares[i][0];
        float nx = block->squares[i][1];

        // Translate to the origin of the block
        ny -= block->cy;
        nx -= block->cx;

        float ny2 = 0.f;
        float nx2 = 0.f;

        // Rotate the square
        if (rotation == Clockwise) {
            ny2 = nx;
            nx2 = -ny;
        } else if (rotation == CounterClockwise) {
            ny2 = -nx;
            nx2 = ny;
        }

        // Translate back
        ny2 += block->cy;
        nx2 += block->cx;

        dst_squares[i][0] = (int)ny2;
        dst_squares[i][1] = (int)nx2;
    }
}
