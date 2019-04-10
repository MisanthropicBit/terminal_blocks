#ifndef TB_HIGHSCORES_H
#define TB_HIGHSCORES_H

static const int TB_MAX_NAME_LENGTH = 50;

typedef struct {
    char name[TB_MAX_NAME_LENGTH];
    int score;
} tb_highscore;

typedef struct {
    int count;
    tb_highscore** records;
} tb_highscores;

// Allocate a new set of highscores
tb_highscores* tb_allocate_highscores(int count);

// Deallocate a set of highscores
void tb_deallocate_highscores(tb_highscores* highscores);

// Load highscores
tb_highscores* tb_load_highscores(const char* const path, int count);

// Save highscores
int tb_save_highscores(const char* const path, tb_highscores* highscores);

// Attempt to insert the name and score if it was a highscore. Returns the
// zero-based position of the insertion in the records or -1 on errors
int tb_record_highscore(const char* const name, int score, tb_highscores* highscores);

// Determine the final position passed on the score or -1 if it did not make the list
int tb_get_highscore_pos(int score, tb_highscores* highscores);

// Draw highscores at a specific point
void tb_draw_highscores(int y, int x, tb_highscores* highscores);

#endif // TB_HIGHSCORES_H
