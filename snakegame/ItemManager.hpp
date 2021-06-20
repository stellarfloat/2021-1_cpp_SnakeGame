// @authors 추헌준(20203155), 한윤석(20203159)

#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

#include <chrono>

#include "MapData.hpp"
#include "GameConfig.hpp"


struct ItemInfo {
  int row;
  int col;
  int info; // 0 GROWTH, 1 POISON
  bool ttl;
  bool first;
  time_t makeTime;
};

class ItemManager {
  ItemInfo itemList[3];
  int rndWeight;
  MapData *map;
public:
  ItemManager(MapData *map);
  void update(time_t t);
  void makeItem(time_t t);
  void delItem(time_t t);
};
#endif
