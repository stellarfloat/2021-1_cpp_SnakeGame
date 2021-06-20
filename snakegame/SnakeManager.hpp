// @authors 추헌준(20203155), 한윤석(20203159)

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
  int itemCountGrowth = 0; 
  int itemCountPoison = 0;
  int gateCount = 0;
  std::pair<int, int> inGateAxis;
  std::deque<std::pair<int, int>> body;
public:
  Snake(MapData *map, int row, int col);

  bool isDead() { return dead; }
  size_t length() { return body.size(); }
  void setDir();
  void update(GateManager& wall);
  int getItemCountGrowth() { return itemCountGrowth; }
  int getItemCountPoison() { return itemCountPoison; }
  int getGateCount() { return gateCount; }
};
#endif
