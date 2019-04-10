#if defined(_WIN) || defined(_WIN32)
    #error "Windows is not supported"
#endif

#include "constants.h"
#include "random.h"
#include "terminal.h"
#include <signal.h>
#include <ncurses.h>

// Initialise the terminal and ncurses
int tb_init_terminal() {
    // Initialise ncurses
    initscr();

    // Initialise colors
    start_color();
    use_default_colors();

    // Disable user input echoing
    noecho();

    // Do not output a newline when pressing enter
    nonl();

    // Get user input immediately, do not
    // wait for a linebreak
    cbreak();

    // The getch function should not block
    //
    // NOTE: Even when the documentation says so, nodelay(window, TRUE) does
    //       not appear to do the same as timeout(0)
    timeout(0);

    // Hide the cursor
    curs_set(0);

    return 0;
}

// Properly shut down ncurses
void tb_close_terminal() {
    endwin();
}

/*void tb_SIGINT_handler(int sig) {*/
/*    // Save stuff*/
/*}*/

/*void tb_SIGWINCH_handler(int sig) {*/
/*    // Adjust to fit layout*/
/*}*/

// Register signal handlers for SIGINT and SIGWINCH
void tb_register_signal_handlers() {
    /*signal(SIGINT, tb_SIGINT_handler);*/
    /*signal(SIGWINCH, tb_SIGWINCH_handler);*/
}

void tb_get_terminal_dimensions(int* width, int* height) {
    // NOTE: getmaxyx is a macro, do not pass pointers
    getmaxyx(stdscr, *width, *height);
}

int tb_get_num_colors_supported() {
    return has_colors() == 0 ? 0 : COLORS;
}

void tb_set_color(int color_index) {
    attron(COLOR_PAIR(color_index));
}

void tb_reset_color(int color_index) {
    attroff(COLOR_PAIR(color_index));
}

void tb_define_colors() {
    int color_indices[TB_MAX_COLOR_ID][3] = {
        {TB_RED_COLOR_ID,     COLOR_RED,     -1},
        {TB_GREEN_COLOR_ID,   COLOR_GREEN,   -1},
        {TB_YELLOW_COLOR_ID,  COLOR_YELLOW,  -1},
        {TB_BLUE_COLOR_ID,    COLOR_BLUE,    -1},
        {TB_MAGENTA_COLOR_ID, COLOR_MAGENTA, -1},
        {TB_WHITE_COLOR_ID,   COLOR_WHITE,   -1},
        {TB_CYAN_COLOR_ID,    COLOR_CYAN,    -1}
    };

    for (int i = 0; i < TB_MAX_COLOR_ID; ++i) {
        init_pair(color_indices[i][0],
                  color_indices[i][1],
                  color_indices[i][2]);
    }
}

void tb_print_colored(int y, int x, char* message, int color_idx) {
    tb_set_color(color_idx);
    mvaddstr(y, x, message);
    tb_reset_color(color_idx);
}

void tb_print_message(const int* const mask,
                      int msg_width,
                      int msg_height,
                      int y,
                      int x) {
    for (int j = 0; j < msg_height; ++j) {
        for (int i = 0; i < msg_width; ++i) {
            int value = mask[j * msg_width + i];

            if (value > 0) {
                tb_set_color(value);
                mvaddch(y + j, x + i, TB_BLOCK_CHAR);
                tb_reset_color(value);
            }
        }
    }
}

void tb_draw_char(int c, int y, int x, int color, int attributes) {
    if (color != -1) {
        tb_set_color(color);
    }

    attron(attributes);
    mvaddch(y, x, c);
    attroff(attributes);

    if (color != -1) {
        tb_reset_color(color);
    }
}

void tb_redraw_screen() {
    refresh();
}

void tb_clear_screen() {
    clear();
}
