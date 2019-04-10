#ifndef TB_TERMINAL_H
#define TB_TERMINAL_H

// Flag for catching and handling process signals
//static volatile sig_atomic_t signalled = 0;

/// Initialise the terminal
int tb_init_terminal();

/// Close the terminal
void tb_close_terminal();

/// Signal handler
void tb_signal_handler(int sig);

/// Register all signal handlers
void tb_register_signal_handlers();

/// Retrieve the terminal's dimensions
void tb_get_terminal_dimensions(int* width, int* height);

/// Get the number of colors supported by the terminal
int tb_get_num_colors_supported();

/// Set the current color
void tb_set_color(int color_index);

/// Reset a color
void tb_reset_color(int color_index);

/// Define all colors needed for the game
void tb_define_colors();

/// Print a message. Its text is given by a mask where
/// each entry is either skipped (zero) or block_char is
/// printed (1)
void tb_print_message(const int* const mask,
                      int msg_width,
                      int msg_height,
                      int width,
                      int height);

// Draw a character
void tb_draw_char(int c, int y, int x, int color, int attributes);

/// Redraw/update the screen
void tb_redraw_screen();

/// Clear the terminal screen
void tb_clear_screen();

#endif // TB_TERMINAL_H
