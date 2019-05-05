#include "constants.h"
#include "enter_highscore.h"
#include "states.h"
#include "terminal.h"
#include <string.h>

int tb_handle_highscore_input(int ch, int* i, char* name) {
    switch (ch) {
        case TB_KEY_ENTER1:
        case TB_KEY_ENTER2:
            if (strlen(name) > 0) {
                return 1;
            }
            break;

        case TB_KEY_DELETE:
            if (*i > 0) {
                name[--(*i)] = '\0';
            }
            break;

        default:
            if (ch != -1 && *i < TB_MAX_NAME_LENGTH) {
                name[(*i)++] = (char)ch;
            }
            break;
    }

    return 0;
}

int tb_enter_highscore(int score, const char* hs_path, tb_highscores* highscores) {
    int pos = tb_get_highscore_pos(score, highscores);

    if (pos == -1) {
        return TB_STATE_MENU;
    }

    // Zero out name buffer and insert a null-terminator at the beginning
    char name[TB_MAX_NAME_LENGTH];
    memset(name, 0, sizeof(char) * TB_MAX_NAME_LENGTH);
    name[0] = '\0';

    int done = 0;
    int i = 0;
    int y = LINES / 2 - TB_GRID_HEIGHT / 2 - 8;
    int x = COLS / 2 - TB_GRID_WIDTH / 2 - 1;

    const char* const place_suffixes[TB_MAX_HIGHSCORES] = {
        "st",
        "nd",
        "rd",
        "th",
        "th"
    };

    werase(stdscr);

    while (!done) {
        int ch = getch();
        done = tb_handle_highscore_input(ch, &i, name);

        mvprintw(y, x, "Congratulations, you got %d%s place", pos, place_suffixes[pos - 1]);
        mvprintw(y + 1, x, "Enter name: %s", name);
        mvprintw(y + 2, x, "%s", "Press [Enter] to confirm\n");

        tb_redraw_screen();
    }

    // Record the new highscore
    if (tb_record_highscore(name, score, highscores) == -1) {
        mvprintw(0, 0, "Failed to save highscore");
        tb_wait_for_key();
    } else {
        // Save the new highscore to file
        if (tb_save_highscores(hs_path, highscores)) {
            mvprintw(0, 0, "Failed to save highscore");
            tb_wait_for_key();
        }
    }

    werase(stdscr);

    return TB_STATE_MENU;
}
