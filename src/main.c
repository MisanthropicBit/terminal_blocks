#include "constants.h"
#include "enter_highscore.h"
#include "game.h"
#include "menu.h"
#include "random.h"
#include "states.h"
#include "terminal.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

/// Return the current user's home directory
char* tb_get_user_home() {
    return getenv("HOME");
}

/// Construct the path to the user's home directory
char* tb_get_highscore_path() {
    char* home = tb_get_user_home();

    if (!home) {
        return NULL;
    }

    int len = strlen(home) + strlen(TB_HIGHSCORE_PATH);
    char* hs_path = malloc(sizeof(char) * (len + 1));

    if (!hs_path) {
        return NULL;
    }

    strcpy(hs_path, home);
    strcat(hs_path, TB_HIGHSCORE_PATH);

    return hs_path;
}

int main() {
    tb_register_signal_handlers();

    if (tb_init_terminal(stdscr) != 0) {
        return 1;
    }

    tb_define_colors();

    // Seed the random number generator
    srand(time(0));

    int running = 1;
    int error = 0;
    int state = TB_STATE_MENU;
    int next_state = 0;
    int final_score = 0;

    // Load highscores
    char* hs_path = tb_get_highscore_path();

    tb_highscores* highscores =
        tb_load_highscores(hs_path, TB_MAX_HIGHSCORES);

    if (!highscores) {
        goto error;
    }

    while (running) {
        switch (state) {
            case TB_STATE_MENU:
                next_state = tb_run_menu(highscores);
                break;

            case TB_STATE_GAME:
                next_state = tb_run_game(&final_score);
                break;

            case TB_STATE_HIGHSCORE:
                next_state = tb_enter_highscore(final_score, hs_path, highscores);
                break;
        }

        if (next_state >= 0) {
            state = next_state;
        } else if (next_state == -1) {
            running = 0;
        }
    }

    error:
    if (hs_path) {
        free(hs_path);
    }

    tb_deallocate_highscores(highscores);
    tb_clear_screen();
    tb_close_terminal();

    return error;
}
