#include <curses.h>
#include <ncurses.h>

// map size definition
#define WIDTH 21
#define HEIGHT 21

// definition for ncurses color pair id
#define COLOR_ID_EMPTY       0
#define COLOR_ID_WALL        1
#define COLOR_ID_IWALL       2
#define COLOR_ID_SNAKE_HEAD  3
#define COLOR_ID_SNAKE_BODY  4
#define COLOR_ID_ITEM_GROWTH 5
#define COLOR_ID_ITEM_POISON 6
#define COLOR_ID_GATE        7

// initialize color pair
// init_pair(pair_id, foreground_color, background_color)
// init_pair(COLOR_ID_EMPTY, COLOR_BLACK, COLOR_BLACK);
// init_pair(COLOR_ID_WALL, COLOR_BLACK, COLOR_WHITE);
// init_pair(COLOR_ID_IWALL, COLOR_BLACK, COLOR_WHITE);
// init_pair(COLOR_ID_SNAKE_HEAD, COLOR_WHITE, COLOR_GREEN);
// init_pair(COLOR_ID_SNAKE_BODY, COLOR_BLACK, COLOR_GREEN);
// init_pair(COLOR_ID_ITEM_GROWTH, COLOR_WHITE, COLOR_YELLOW);
// init_pair(COLOR_ID_ITEM_POISON, COLOR_WHITE, COLOR_RED);
// init_pair(COLOR_ID_GATE, COLOR_WHITE, COLOR_MAGENTA);

