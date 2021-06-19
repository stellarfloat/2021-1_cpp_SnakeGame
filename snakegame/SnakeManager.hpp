#ifndef _SNAKEMANAGER_H_
#define _SNAKEMANAGER_H_

#include <deque>

#include "MapData.hpp"


class Snake {
  MapData *map;
  bool dead = false;
  char dir;
  int size, headRow, headCol;
  std::deque<std::pair<int, int>> body;
public:
  Snake(MapData *map, int row = 10, int col = 9);

  bool isDead() { return dead; }
  void setDir();
  void update();
};
#endif
