// @author 추헌준(20203155)

#ifndef _GAMECONFIG_H_
#define _GAMECONFIG_H_

#include <curses.h>
#include <ncurses.h>

// map size definition
#define WIDTH 32
#define HEIGHT 32

// definition for ncurses color pair id
#define COLOR_ID_EMPTY       0
#define COLOR_ID_WALL        1
#define COLOR_ID_IWALL       2
#define COLOR_ID_SNAKE_HEAD  3
#define COLOR_ID_SNAKE_BODY  4
#define COLOR_ID_ITEM_GROWTH 5
#define COLOR_ID_ITEM_POISON 6
#define COLOR_ID_GATE        7

// maximum item count on the map
#define MAX_ITEM_SIZE 3

// gametick length (ms)
#define GAMETICK_DELAY 250

// gate movement ID
const std::pair<int, int> axisID[] = {
  std::make_pair(0, -1), 
  std::make_pair(-1, 0), 
  std::make_pair(0, 1), 
  std::make_pair(1, 0)
};
#endif
