#include "constants.h"
#include "enter_highscore.h"
#include "states.h"

int tb_enter_highscore(int score, tb_highscores* highscores) {
    int pos = tb_get_highscore_pos(score, highscores);

    if (pos == -1) {
        return TB_STATE_MENU;
    }

    int done = 0;
    char name[TB_MAX_NAME_LENGTH];
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

    while (!done) {
        int ch = getch();

        if (ch != -1) {
            if (ch == TB_KEY_CONFIRM) {
                done = 1;
            } else if (ch == TB_KEY_DELETE) {
                name[i--] = ' ';
            } else {
                if (i < TB_MAX_NAME_LENGTH) {
                    name[i++] = (char)ch;
                }
            }
        }

        mvprintw(y, x, "You got %d%s place", pos, place_suffixes[pos]);
        mvprintw(y + 1, x, "Enter name: %s", name);
        werase(stdscr);
    }

    // Record the new highscore
    if (tb_record_highscore(name, score, highscores) == -1) {
        mvprintw(0, 0, "Failed to save highscore");
    } else {
        // Save the new highscore to file
        if (tb_save_highscores(TB_HIGHSCORE_PATH, highscores)) {
            mvprintw(0, 0, "Failed to save highscore");
        }
    }

    return TB_STATE_MENU;
}
