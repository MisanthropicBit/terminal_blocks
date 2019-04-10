#include "constants.h"
#include "enter_highscore.h"
#include "game.h"
#include "menu.h"
#include "random.h"
#include "states.h"
#include "terminal.h"
#include <time.h>
#include <stdlib.h>

int main() {
    tb_register_signal_handlers();

    if (tb_init_terminal(stdscr) != 0) {
        printf("Failed to initialise terminal\n");
        return 1;
    }

    tb_define_colors();

    // Seed the random number generator
    srand(time(0));

    int running = 1;
    int state = TB_STATE_MENU;
    int next_state = 0;
    int final_score = 0;

    // Load highscores
    tb_highscores* highscores =
        tb_load_highscores(TB_HIGHSCORE_PATH, TB_MAX_HIGHSCORES);

    if (!highscores) {
        printf("%s", "Failed to load highscores");
        tb_close_terminal();
        return 1;
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
                next_state = tb_enter_highscore(final_score, highscores);
                break;
        }

        if (next_state > 0) {
            state = next_state;
            tb_clear_screen();
        } else if (next_state == -1) {
            running = 0;
        }
    }

    tb_deallocate_highscores(highscores);
    tb_clear_screen();
    tb_close_terminal();

    return 0;
}
