#include "constants.h"
#include "highscores.h"
#include "menu.h"
#include "terminal.h"
#include <pwd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void tb_display_menu_title(int y, int x) {
    /*attron(A_BOLD);*/
    tb_print_message(TB_TITLE_MASK,
                     TB_TITLE_WIDTH,
                     TB_TITLE_HEIGHT,
                     y,
                     x);
    /*attroff(A_BOLD);*/

    // Display the current version
    mvprintw(y + TB_TITLE_HEIGHT + 2,
             x + TB_TITLE_WIDTH/2 - 6,
             "Version %d.%d.%d",
             TB_VERSION_MAJOR,
             TB_VERSION_MINOR,
             TB_VERSION_REVISION);

    // Display the number of supported colors (not reliable)
    mvprintw(y + TB_TITLE_HEIGHT + 3,
             x + TB_TITLE_WIDTH/2 - 5,
             "Colors: %d",
             tb_get_num_colors_supported());
}

/// Return the current user's home directory
char* tb_get_user_home() {
    char* user_home = getenv("HOME");

    /*if (!user_home) {*/
    /*    struct passwd* pw = getpwuid(getuid());*/

    /*    return pw->pw_dir ? pw->pw_dir : NULL;*/
    /*}*/

    return user_home;
}

/// Move and redraw the cursor
void tb_move_cursor(int x, int delta, int cursor, int attributes, int option_start,
                    int option_spacing, int option_count, int* option_idx) {
    int y = option_start + *option_idx * option_spacing;

    // Clear the cursor's previous position
    mvaddch(y, x, ' ');

    // Update the option index (currently selected option) and the vertical
    // position of the cursor
    *option_idx += delta;

    // Clamp the newly computed option index
    if (*option_idx < 0) {
        *option_idx = option_count - 1;
    } else if (*option_idx >= option_count) {
        *option_idx = 0;
    }

    // Compute the new position of the cursor
    y = option_start + *option_idx * option_spacing;

    // Draw the cursor at its new position with any requested attributes
    attron(attributes);
    mvaddch(y, x, cursor);
    attroff(attributes);
}

int tb_run_menu(tb_highscores* highscores) {
    if (!highscores) {
        return -1;
    }

    int running = 1;
    int quit = 0;
    int next_state = 0;
    int cursor_x = COLS/2 - 4;
    int option_idx = 0;
    const char* const options[] = {"[P]lay", "[Q]uit"};
    const int option_count = 2;
    const int option_start = LINES/2 + 5;
    const int option_spacing = 2;
    char chr = 0;

    // Display menu title, options and cursor
    tb_display_menu_title(LINES/2 - TB_TITLE_HEIGHT, COLS/2 - TB_TITLE_WIDTH/2);

    for (int i = 0; i < option_count; ++i) {
        mvprintw(option_start + i * option_spacing, COLS/2 - 2, options[i]);
    }

    tb_move_cursor(cursor_x, 0, TB_CURSOR_CHAR, A_BOLD, option_start,
                   option_spacing, option_count, &option_idx);

    while (running) {
        chr = getch();

        #if TB_DEBUG
        mvprintw(0, 0, "Key: %d ", chr);
        #endif

        tb_draw_highscores(option_start + option_spacing * option_count,
                           COLS/2 - 2, highscores);

        switch (chr) {
            case TB_KEY_DOWN:
                tb_move_cursor(cursor_x, 1, TB_CURSOR_CHAR, A_BOLD, option_start,
                               option_spacing, option_count, &option_idx);
                break;

            case TB_KEY_UP:
                tb_move_cursor(cursor_x, -1, TB_CURSOR_CHAR, A_BOLD, option_start,
                               option_spacing, option_count, &option_idx);
                break;

            case TB_KEY_CONFIRM:
                if (option_idx == 0) {
                    running = 0;
                    next_state = 2;
                } else if (option_idx == 1) {
                    running = 0;
                    quit = 1;
                }
                break;

            case TB_KEY_PAUSE:
                // Used for '[P]lay' to start the game, but for pausing when
                // inside the game
                running = 0;
                next_state = 2;
                break;

            case TB_KEY_QUIT:
                running = 0;
                quit = 1;
                break;

            default:
                break;
        }

        tb_redraw_screen();
    }

    return quit ? -1 : next_state;
}
