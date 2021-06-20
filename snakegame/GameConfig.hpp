// @author 추헌준(20203155)

#ifndef _GAMECONFIG_H_
#define _GAMECONFIG_H_

#include <string>
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


// gate movement ID
const std::pair<int, int> axisID[] = {
  std::make_pair(0, -1), 
  std::make_pair(-1, 0), 
  std::make_pair(0, 1), 
  std::make_pair(1, 0)
};

// definition of mission in each level
// {bodyLength, itemGrowth, itemPoison, gateUsed}
const int levelMissionData[5][4] = {
  {0, 0, 0, 0},  // index 0 is empty
  {7, 1, 1, 1},  // level 1, map1.txt
  {9, 3, 3, 2},  // level 2, map2.txt
  {11, 5, 5, 3}, // level 3, map3.txt
  {11, 7, 7, 4}, // level 4, map4.txt
};

// level metadata
// (levelPath, snakePos)
const std::pair<std::string, std::pair<int, int>> levelMetaData[] = {
  std::make_pair("snakegame/LevelData/blank.txt", std::make_pair(0, 0)), // startscreen
  std::make_pair("snakegame/LevelData/map1.txt", std::make_pair(3, 13)), 
  std::make_pair("snakegame/LevelData/map2.txt", std::make_pair(16, 22)), 
  std::make_pair("snakegame/LevelData/map3.txt", std::make_pair(16, 18)), 
  std::make_pair("snakegame/LevelData/map4.txt", std::make_pair(3, 13))
};
#endif
