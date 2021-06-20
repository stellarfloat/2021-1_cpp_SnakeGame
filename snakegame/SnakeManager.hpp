#ifndef _SNAKEMANAGER_H_
#define _SNAKEMANAGER_H_

#include <deque>

#include "GameConfig.hpp"
#include "GateManager.hpp"
#include "MapData.hpp"


class Snake {
  MapData *map;
  bool dead = false;
  char dir; // 0: left, 1:up, 2:right, 3.down
  int size, headRow, headCol;
  std::pair<int, int> inGateAxis;
  std::deque<std::pair<int, int>> body;
public:
  Snake(MapData *map, int row = 16, int col = 14);

  bool isDead() { return dead; }
  size_t length() { return body.size(); }
  void setDir();
  void update(GateManager& wall);
};
#endif
