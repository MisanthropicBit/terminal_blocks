#include "constants.h"
#include "game.h"
#include "random.h"
#include "states.h"
#include "terminal.h"
#include "timer.h"
#include <ncurses.h>
#include <stdlib.h>

int tb_color_from_block_type(int type) {
    return type - 1;
}

tb_block* tb_spawn_block(int y, int x) {
    int type = tb_random_number(1, TB_BLOCK_TYPES);
    int color = type;
    int rotations = tb_random_number(0, 3);
    tb_block* block = tb_allocate_block(y, x, type, rotations, color);

    // Randomly rotate the block
    for (int i = 0; i < rotations; ++i) {
        tb_rotate_squares(block,
                          block->squares,
                          Clockwise);
    }

    return block;
}

// Save the squares of the focus block in the game grid
void tb_finalise_focus_block(const tb_block* const focus_block,
                             tb_game_grid* grid) {
    int block_type = focus_block->type;

    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        // Convert square positions to zero-based grid coordinates
        int gy = focus_block->squares[i][0] - grid->y;
        int gx = focus_block->squares[i][1] - grid->x;

        grid->grid[gy * grid->width + gx] = block_type;
    }
}

void tb_deallocate_game_grid(tb_game_grid* grid) {
    if (grid) {
        if (grid->grid) {
            free(grid->grid);
        }

        free(grid);
    }
}

tb_game_grid* tb_allocate_game_grid(int y, int x, int height, int width) {
    tb_game_grid* grid = malloc(sizeof(tb_game_grid));

    if (grid) {
        grid->y = y;
        grid->x = x;
        grid->height = height;
        grid->width = width;
        grid->grid = calloc(height * width, sizeof(tb_block_type));

        if (!grid->grid) {
            tb_deallocate_game_grid(grid);
            return NULL;
        }
    }

    return grid;
}

void tb_draw_grid(tb_game_grid* grid, int block_char) {
    attron(A_BOLD);

    for (int y = 0; y < grid->height; ++y) {
        for (int x = 0; x < grid->width; ++x) {
            int sy = grid->y + y;
            int sx = grid->x + x;
            tb_block_type cell = grid->grid[y * grid->width + x];

            if (cell != 0) {
                tb_set_color(cell);
                mvaddch(sy, sx, block_char);
                tb_reset_color(cell);
            } else {
                mvaddch(sy, sx, ' ');
            }

            #ifdef TB_DEBUG
            mvprintw(y, x, "%d", cell);
            #endif
        }
    }

    attroff(A_BOLD);
}

// Clear filled lines and move partially filled lines down
int tb_clear_lines(tb_game_grid* grid) {
    int lines_cleared = 0;

    // Check for full lines bottom-up
    for (int line = grid->height - 1; line >= 0; --line) {
        int cols = 0;

        // Count the number of filled positions in the row
        for (int col = 0; col < grid->width; ++col) {
            if (grid->grid[line * grid->width + col] > 0) {
                // The position is non-empty
                ++cols;
            }
        }

        if (cols == grid->width) {
            // All column positions were full
            ++lines_cleared;

            // Clear the line/row
            for (int col = 0; col < grid->width; ++col) {
                grid->grid[line * grid->width + col] = 0;
            }
        } else if (lines_cleared > 0) {
            // This is either an empty line or a non-empty, partially filled
            // line, so move it down
            for (int col = 0; col < grid->width; ++col) {
                grid->grid[(line + lines_cleared) * grid->width + col] =
                    grid->grid[line * grid->width + col];
            }

            // If the line was empty, end the search
            if (cols == 0) {
                break;
            }
        }
    }

    return lines_cleared;
}

void tb_print_game_over_message() {
    tb_clear_screen();

    tb_print_message(TB_GAME_OVER_MSG_MASK,
                     TB_GAME_OVER_MSG_WIDTH,
                     TB_GAME_OVER_MSG_HEIGHT,
                     COLS,
                     LINES);
}

void tb_get_move_deltas(tb_direction direction, int* delta_y, int* delta_x) {
    switch (direction) {
        case Left:
            *delta_y = 0;
            *delta_x = -TB_SQUARE_SIZE;
            break;

        case Right:
            *delta_y = 0;
            *delta_x = TB_SQUARE_SIZE;
            break;

        case Down:
            *delta_y = TB_SQUARE_SIZE;
            *delta_x = 0;
            break;

        default:
            *delta_y = 0;
            *delta_x = 0;
            break;
    }
}

// Check for any block collisions in a given direction
int tb_check_block_collisions(int squares[4][2],
                              tb_direction direction,
                              tb_game_grid* grid) {
    int delta_y = 0, delta_x = 0;
    tb_get_move_deltas(direction, &delta_y, &delta_x);

    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        int sy = squares[i][0] + delta_y - grid->y;
        int sx = squares[i][1] + delta_x - grid->x;
        /*mvprintw(20 + i, 0, "%d", sy * grid->width + sx);*/
        /*mvprintw(20, 0, "%d, %d (%d)     ", sy, sx, sy * grid->width + sx);*/

        if (grid->grid[sy * grid->width + sx] != 0) {
            return 1;
        }
    }

    return 0;
}

// Check for wall collisions in a given direction
int tb_check_wall_collisions(int squares[4][2],
                             tb_direction direction,
                             tb_game_grid* grid) {
    int delta_y = 0, delta_x = 0;
    tb_get_move_deltas(direction, &delta_y, &delta_x);

    // Compute the bounds of the grid
    int ymax = grid->y + grid->height - 1;
    int xmin = grid->x;
    int xmax = grid->x + grid->width - 1;

    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        int sy = squares[i][0] + delta_y;
        int sx = squares[i][1] + delta_x;

        if (sy > ymax || sx < xmin || sx > xmax) {
            return 1;
        }
    }

    return 0;
}

// Moves the focus block all the way down, essentially "dropping" it
// from its current position
void tb_drop_block(tb_block* const focus_block, tb_game_grid* grid) {
    if (!focus_block || !grid || !grid->grid) {
        return;
    }

    while (!tb_check_wall_collisions(focus_block->squares, Down, grid) &&
           !tb_check_block_collisions(focus_block->squares, Down, grid)) {
        tb_clear_block(focus_block);
        tb_move_block(focus_block, 1, 0);
    }
}

// NOTE: This is a good candidate to showcase DRY + refactoring, by
// passing squares to the collision functions instead of blocks
int tb_check_rotation_collisions(tb_block* focus_block,
                                 tb_rotation rotation,
                                 tb_game_grid* grid,
                                 int rotated_squares[4][2]) {
    // Rotate the focus block's squares
    tb_rotate_squares(focus_block, rotated_squares, rotation);

    return tb_check_wall_collisions(rotated_squares, None, grid) ||
           tb_check_block_collisions(rotated_squares, None, grid);
}

// Handle user input when the game is being played
int tb_handle_game_input(tb_block* const focus_block,
                         tb_game_grid* grid,
                         int game_over,
                         int* draw_guide,
                         int* pause) {
    int ch = getch();

    #if TB_DEBUG
    if (ch != -1) {
        mvprintw(0, 0, "       ");
        mvprintw(0, 0, "%c", (char)ch);
    }
    #endif

    if (game_over) {
        if (ch != -1) {
            return 1;
        }
    } else {
        switch (ch) {
            case TB_KEY_LEFT:
                if (!tb_check_wall_collisions(focus_block->squares, Left, grid) &&
                    !tb_check_block_collisions(focus_block->squares, Left, grid)) {
                        tb_clear_block(focus_block);
                        tb_move_block(focus_block, 0, -1);
                }
                break;

            case TB_KEY_RIGHT:
                if (!tb_check_wall_collisions(focus_block->squares, Right, grid) &&
                    !tb_check_block_collisions(focus_block->squares, Right, grid)) {
                        tb_clear_block(focus_block);
                        tb_move_block(focus_block, 0, 1);
                }
                break;

            case TB_KEY_DOWN:
                if (!tb_check_wall_collisions(focus_block->squares, Down, grid) &&
                    !tb_check_block_collisions(focus_block->squares, Down, grid)) {
                        tb_clear_block(focus_block);
                        tb_move_block(focus_block, 1, 0);
                }
                break;

            case TB_KEY_ROTATE_LEFT:
            case TB_KEY_ROTATE_RIGHT:
                {
                    int rotated_squares[TB_SQUARES_PER_BLOCK][2];
                    tb_rotation rotation = ch == TB_KEY_ROTATE_LEFT ? CounterClockwise : Clockwise;

                    if (!tb_check_rotation_collisions(focus_block, rotation,
                                                      grid, rotated_squares)) {
                        for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
                            mvaddch(focus_block->squares[i][0], focus_block->squares[i][1], ' ');

                            focus_block->squares[i][0] = rotated_squares[i][0];
                            focus_block->squares[i][1] = rotated_squares[i][1];
                        }
                    }
                }
                break;

            case TB_KEY_CONFIRM:
                tb_drop_block(focus_block, grid);
                break;

            case TB_KEY_TOGGLE_GUIDES:
                *draw_guide = !*draw_guide;
                break;

            case TB_KEY_PAUSE:
                *pause = !*pause;
                break;

            case TB_KEY_QUIT:
                return 1;

            default:
                break;
        }
    }

    return 0;
}

int tb_check_game_over(tb_block* focus_block,
                       tb_game_grid* grid) {
    // The game is over if the focus block's starting
    // position collides with an existing square
    return tb_check_block_collisions(focus_block->squares, None, grid);
}

// Draw visual guides for where a block will be placed if dropped
void tb_draw_guides(tb_block* block, tb_game_grid* grid) {
    int squares[4][2];

    // Copy the current square positions
    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        squares[i][0] = block->squares[i][0];
        squares[i][1] = block->squares[i][1];
    }

    // Move the squares down until we hit something
    while (!tb_check_block_collisions(squares, Down, grid)
           && !tb_check_wall_collisions(squares, Down, grid)) {
        for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
            ++squares[i][0];
        }
    }

    // Draw the guide squares if they do not collide with the actual block's
    // squares
    for (int i = 0; i < TB_SQUARES_PER_BLOCK; ++i) {
        if (squares[i][0] != block->squares[i][0]
                || squares[i][1] != block->squares[i][1]) {
            tb_draw_char(TB_BLOCK_CHAR, squares[i][0], squares[i][1], block->color, A_DIM);
        }
    }
}

void tb_draw_box(int y, int x, int height, int width, int left_char,
                 int right_char, int top_char, int bottom_char,
                 int tl_char, int bl_char, int br_char, int tr_char) {
    // Draw left border
    for (int j = y; j < y + height; ++j) {
        mvaddch(j, x, left_char);
    }

    // Draw right border
    for (int j = y; j < y + height; ++j) {
        mvaddch(j, x + width, right_char);
    }

    // Draw top border
    for (int i = x; i < x + width; ++i) {
        mvaddch(y, i, top_char);
    }

    // Draw bottom border
    for (int i = x; i < x + width; ++i) {
        mvaddch(y + height, i, bottom_char);
    }

    // Draw corners
    mvaddch(y, x, tl_char);
    mvaddch(y + height, x, bl_char);
    mvaddch(y + height, x + width, br_char);
    mvaddch(y, x + width, tr_char);
}

int tb_force_move_block(tb_game_grid* grid, tb_block* focus_block) {
    if (tb_check_wall_collisions(focus_block->squares, Down, grid) ||
            tb_check_block_collisions(focus_block->squares, Down, grid)) {
        // We hit something, return 1 to signal to start sliding
        return 1;
    }

    // We did not hit anything so force move the focus block
    tb_clear_block(focus_block);
    tb_move_block(focus_block, 1, 0);

    return 0;
}

void tb_draw_game(tb_game_grid* grid, tb_block* focus_block,
                  tb_block* next_block, int score, int draw_guide,
                  int pause, int game_over, int score_y, int score_x,
                  int guide_y, int guide_x, int pause_y, int pause_x) {
    if (!grid) {
        return;
    }

    if (game_over) {
        mvprintw(grid->y - 6, grid->x, "Game Over!");
        mvprintw(grid->y - 5, grid->x, "Press any key to continue...");
    }

    // Draw the game grid boundary
    tb_draw_box(grid->y, grid->x - 1, grid->height, grid->width + 1,
                TB_GRID_SIDE_CHAR,
                TB_GRID_SIDE_CHAR,
                ' ',
                TB_GRID_BOTTOM_CHAR,
                TB_GRID_SIDE_CHAR,
                TB_GRID_BOTTOMLEFT_CHAR,
                TB_GRID_BOTTOMRIGHT_CHAR,
                TB_GRID_SIDE_CHAR);

    // Draw the grid
    tb_draw_grid(grid, TB_BLOCK_CHAR);

    if (draw_guide) {
        // Draw the guides that show where the block will land
        tb_draw_guides(focus_block, grid);
    }

    mvprintw(guide_y, guide_x, "Guides %s", draw_guide ? "enabled" : "disabled");

    if (pause) {
        mvprintw(pause_y, pause_x, "Game paused");
    }

    // Draw the focus block
    tb_draw_block(focus_block);

    // Draw the next block and the current score and level
    // We print the score on the right side to avoid having
    // to move it left as the score grows
    tb_draw_block(next_block);
    mvprintw(score_y, score_x, "Score: %d", score);
}

int tb_run_game(int* const final_score) {
    *final_score = 0;
    int score = 0;
    int game_over = 0;
    int draw_guide = 1;
    int pause = 0;
    int running = 1;
    tb_time_unit move_time = tb_current_time();
    tb_time_unit slide_time = 0;
    int sliding = 0;

    // Allocate the game area
    tb_game_grid* grid = tb_allocate_game_grid(LINES / 2 - TB_GRID_HEIGHT / 2,
                                               COLS / 2 - TB_GRID_WIDTH / 2 - 1,
                                               TB_GRID_HEIGHT,
                                               TB_GRID_WIDTH);

    if (!grid) {
        return TB_STATE_MENU;
    }

    // Compute the bounds of the game and the positions
    // of the blocks and text
    int next_block_y = grid->y + 2;
    int next_block_x = grid->x + TB_GRID_WIDTH + 4;
    int score_y = next_block_y + 4;
    int score_x = next_block_x;
    int guide_y = score_y + 2;
    int guide_x = score_x;
    int pause_y = guide_y + 2;
    int pause_x = guide_x;

    tb_block* focus_block = tb_spawn_block(grid->y, grid->x + TB_GRID_WIDTH/2);
    tb_block* next_block = tb_spawn_block(next_block_y, next_block_x);

    if (!focus_block || !next_block) {
        return TB_STATE_MENU;
    }

    while (running) {
        if(tb_handle_game_input(focus_block, grid, game_over, &draw_guide, &pause)) {
            break;
        }

        werase(stdscr);

        if (pause) {
            // Do not update the game when paused
        } else if (!game_over) {
            if (!sliding) {
                // Check if it is time to forcefully move the focus block
                if (tb_current_time() - move_time >= TB_MOVE_DELAY) {
                    sliding = tb_force_move_block(grid, focus_block);

                    if (sliding) {
                        slide_time = tb_current_time();
                    } else {
                        move_time = tb_current_time();
                    }
                }
            } else {
                // Check if we should stop sliding
                if (tb_current_time() - slide_time >= TB_SLIDE_TIME) {
                    // Store the focus block in the grid, then deallocate it
                    tb_finalise_focus_block(focus_block, grid);
                    tb_deallocate_block(focus_block);

                    // Set the focus block to the next block and find
                    // a new next block
                    focus_block = next_block;

                    // Set the position of the new focus block and spawn a new
                    // next block
                    tb_set_block(focus_block, grid->y - 1, grid->x + grid->width/2);
                    next_block = tb_spawn_block(next_block_y, next_block_x);

                    // Stop sliding
                    sliding = 0;

                    // Attempt to clear any lines
                    score += tb_clear_lines(grid) * TB_SCORE_PER_ROW;

                    // Check if the new focus block immediately collides with
                    // anything
                    game_over = tb_check_game_over(focus_block, grid);

                    // Reset the move time to the current time
                    move_time = tb_current_time();
                } else {
                    // If we are not yet done with sliding, check to see if we are
                    // still colliding. If we are not, then the player must have
                    // moved the focus block out of the collision and we should
                    // reset the slide timer
                    if (!tb_check_wall_collisions(focus_block->squares, Down, grid) &&
                        !tb_check_block_collisions(focus_block->squares, Down, grid)) {
                            sliding = 0;
                            move_time = tb_current_time();
                    }
                }
            }
        }

        tb_draw_game(grid, focus_block, next_block, score, draw_guide,
                     pause, game_over, score_y, score_x, guide_y, guide_x,
                     pause_y, pause_x);
    }

    // Deallocate memory
    tb_deallocate_block(focus_block);
    tb_deallocate_block(next_block);
    tb_deallocate_game_grid(grid);

    // Save the final score for the highscore state
    *final_score = score;

    return TB_STATE_HIGHSCORE;
}
