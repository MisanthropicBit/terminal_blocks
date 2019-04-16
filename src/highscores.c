#include "assert.h"
#include "constants.h"
#include "highscores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tb_highscores* tb_allocate_highscores(int count) {
    if (count < 1) {
        return NULL;
    }

    tb_highscores* highscores = malloc(sizeof(tb_highscores));
    highscores->count = count;

    if (!highscores) {
        return NULL;
    }

    highscores->records = malloc(sizeof(tb_highscore*) * count);

    if (!highscores->records) {
        goto error;
    }

    for (int i = 0; i < count; ++i) {
        tb_highscore* highscore = malloc(sizeof(tb_highscore));

        if (!highscore) {
            goto error;
        }

        memset(highscore->name, 0, TB_MAX_NAME_LENGTH);
        highscore->score = 0;

        highscores->records[i] = highscore;
    }

    return highscores;

    error:
    tb_deallocate_highscores(highscores);
    return NULL;
}

void tb_deallocate_highscores(tb_highscores* highscores) {
    if (!highscores) {
        return;
    }

    for (int i = 0; i < highscores->count; ++i) {
        if (highscores->records[i]) {
            free(highscores->records[i]);
            highscores->records[i] = NULL;
        }
    }

    free(highscores);
    highscores = NULL;
}

tb_highscores* tb_load_highscores(const char* const path, int count) {
    tb_highscores* highscores = tb_allocate_highscores(count);

    if (!path || !highscores || count < 1) {
        return NULL;
    }

    // Open the file in append/update mode. This will create the file if it does not already
    // exist and write by appending to the file (which is irrelevant since we are only
    // reading)
    FILE* file = fopen(path, "a+");

    if (!file) {
        return NULL;
    }

    fseek(file, SEEK_SET, 0);

    // Otherwise, we read the file as normal
    int record_id = 0;
    char name[TB_MAX_NAME_LENGTH];
    int score = -1;

    while (record_id < TB_MAX_HIGHSCORES) {
        int n = fscanf(file, "%s %d\n", name, &score);

        if (n != 2) {
            break;
        }

        tb_highscore* highscore = highscores->records[record_id];

        // Copy the name into the record
        strncpy(highscore->name, name, TB_MAX_NAME_LENGTH);

        // Ensure that the name is null-terminated
        highscore->name[TB_MAX_NAME_LENGTH - 1] = '\0';
        highscore->score = score;

        ++record_id;
    }

    if (record_id == 0) {
        // No records were read, assume the file was created anew and write the
        // default values to the file
        for (int i = 0; i < TB_MAX_HIGHSCORES; ++i) {
            fprintf(file, "%s %d\n",
                    TB_DEFAULT_HIGHSCORE_NAMES[i],
                    TB_DEFAULT_HIGHSCORE_SCORES[i]);

            tb_highscore* highscore = highscores->records[i];
            strncpy(highscore->name, TB_DEFAULT_HIGHSCORE_NAMES[i], TB_MAX_NAME_LENGTH);
            highscore->score = TB_DEFAULT_HIGHSCORE_SCORES[i];
        }
    } else if (record_id != TB_MAX_HIGHSCORES) {
        return NULL;
    }

    fclose(file);

    return highscores;
}

int tb_save_highscores(const char* const path,
                       tb_highscores* highscores) {
    if (!path || !highscores) {
        return 1;
    }

    FILE* file = fopen(path, "w");

    for (int i = 0; i < highscores->count; ++i) {
        fprintf(file, "%s:%d", highscores->records[i]->name, highscores->records[i]->score);
    }

    fclose(file);

    return 0;
}

int tb_record_highscore(const char* const name, int score, tb_highscores* highscores) {
    if (!name || score < 1 || !highscores) {
        return -1;
    }

    if (strlen(name) >= TB_MAX_NAME_LENGTH) {
        return -1;
    }

    int record_count = 0;

    // Find the position of the new record
    int insertion_idx = tb_get_highscore_pos(score, highscores);

    // Insert the new record if it beat any previous record
    if (insertion_idx != -1) {
        tb_highscore* highscore = malloc(sizeof(tb_highscore));

        if (!highscore) {
            return -1;
        }

        // Move other records down
        for (int i = record_count - 1; i >= insertion_idx; --i) {
            highscores->records[i - 1] = highscores->records[i];
        }

        // Insert the record at its final position
        strcpy(highscore->name, name);
        highscore->score = score;
        highscores->records[insertion_idx] = highscore;
    }

    return insertion_idx;
}

int tb_get_highscore_pos(int score, tb_highscores* highscores) {
    if (!highscores) {
        return -1;
    }

    int pos = -1;

    // Find the position of the new record
    for (int i = highscores->count - 1; i >= 0; pos = i--) {
        if (score <= highscores->records[i]->score) {
            break;
        }
    }

    return pos;
}

void tb_draw_highscores(int y, int x, tb_highscores* highscores) {
    if (!highscores) {
        return;
    }

    char string_score[50];

    for (int i = 0; i < highscores->count; ++i) {
        if (sprintf(string_score, "%d", highscores->records[i]->score) < 0) {
            mvprintw(y + i * 2, x / 2, "Error copying score, buffer too small");
        }

        size_t len = strlen(string_score) + strlen(highscores->records[i]->name) + 3;

        mvprintw(y + i * 2, x - len / 2, "%d. %s - %d",
                 i + 1, highscores->records[i]->name, highscores->records[i]->score);
    }
}
