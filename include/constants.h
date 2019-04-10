#ifndef TB_CONSTANTS_H
#define TB_CONSTANTS_H

#include <ncurses.h>

static const int TB_VERSION_MAJOR    = 0;
static const int TB_VERSION_MINOR    = 1;
static const int TB_VERSION_REVISION = 0;

#define TB_BLOCK_CHAR  '#'
#define TB_CURSOR_CHAR '>'

// Characters used for drawing the game grid's boundary
#define TB_GRID_SIDE_CHAR        ACS_VLINE
#define TB_GRID_TOPLEFT_CHAR     ACS_ULCORNER
#define TB_GRID_TOPRIGHT_CHAR    ACS_URCORNER
#define TB_GRID_BOTTOM_CHAR      ACS_HLINE
#define TB_GRID_BOTTOMLEFT_CHAR  ACS_LLCORNER
#define TB_GRID_BOTTOMRIGHT_CHAR ACS_LRCORNER

static const int TB_RED_COLOR_ID     = 1;
static const int TB_GREEN_COLOR_ID   = 2;
static const int TB_YELLOW_COLOR_ID  = 3;
static const int TB_BLUE_COLOR_ID    = 4;
static const int TB_MAGENTA_COLOR_ID = 5;
static const int TB_WHITE_COLOR_ID   = 6;
static const int TB_CYAN_COLOR_ID    = 7;

static const int TB_MAX_HIGHSCORES = 5;
static const int TB_MAX_COLOR_ID   = 7;
static const int TB_TITLE_WIDTH    = 82;
static const int TB_TITLE_HEIGHT   = 17;

// Mask for the main game title spelling "TERMINAL BLOCKS" on two lines. Each
// number is a color code. We make each letter wider than taller since
// characters on screen are taller than they are wider
static const int TB_TITLE_MASK[TB_TITLE_HEIGHT * TB_TITLE_WIDTH] = {
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 3, 3, 3, 0, 0,
    0, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 4,
    4, 4, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 7,
    7, 7, 7, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0,
    3, 3, 3, 3, 3, 3, 0, 0, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 6, 6, 6, 6,
    6, 6, 0, 0, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5,
    5, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 0, 4, 4, 4, 0,
    0, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 0, 0, 3, 0, 0, 0, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 4, 4, 4, 0, 4,
    4, 0, 0, 4, 4, 4, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 7, 7, 7, 7, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 3, 3,
    3, 3, 0, 0, 5, 5, 5, 5, 0, 5, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 4, 4, 0, 4, 4, 4, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
    0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 6, 6, 6,
    0, 0, 0, 0, 6, 6, 6, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 5, 5, 5, 5, 0, 0, 0,
    5, 5, 5, 5, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 4, 4, 4, 0, 0, 0, 0, 4, 4,
    4, 4, 0, 0, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0,
    5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 4, 4,
    4, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 0, 0, 7, 7,
    7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 1, 1,
    1, 0, 0, 1, 1, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0,
    2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 3, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1,
    0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 3, 3, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 2, 2, 2, 2,
    2, 2, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 3,
    3, 3, 3, 3, 3, 3, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0,
    3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const int TB_SQUARES_PER_BLOCK = 4;
static const int TB_BLOCK_TYPES = 7;
static const int TB_SQUARE_SIZE = 1;

static const int TB_GRID_WIDTH = 20;
static const int TB_GRID_HEIGHT = 15;

// Score for clearing a single row
static const int TB_SCORE_PER_ROW = 20;

// Elapsed milliseconds between moving the current block
static const int TB_MOVE_DELAY = 1000;

// Milliseconds for sliding a block after it collides
static const int TB_SLIDE_TIME = 1000;

static const int TB_GAME_OVER_MSG_HEIGHT = 5;
static const int TB_GAME_OVER_MSG_WIDTH  = 31;

static const int TB_GAME_OVER_MSG_MASK[TB_GAME_OVER_MSG_HEIGHT][TB_GAME_OVER_MSG_WIDTH] = {
    {0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1}
};

// User input keys
static const int TB_KEY_LEFT         = 'a';
static const int TB_KEY_DOWN         = 's';
static const int TB_KEY_RIGHT        = 'd';
static const int TB_KEY_UP           = 'w';
static const int TB_KEY_CONFIRM      = 'f';
static const int TB_KEY_DELETE       = 127; // Backspace
static const int TB_KEY_ROTATE_LEFT  = 'j';
static const int TB_KEY_ROTATE_RIGHT = 'k';
static const int TB_KEY_PAUSE        = 'p';
static const int TB_KEY_QUIT         = 'q';

static const char* TB_HIGHSCORE_PATH = "./highscores.txt";

#endif // TB_CONSTANTS_H
