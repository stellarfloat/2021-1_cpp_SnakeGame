#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

#include <chrono>

#include "MapData.hpp"


struct ItemInfo {
  int row;
  int col;
  int info; // 0 GROWTH, 1 POISON
  bool ttl;
  bool first;
  clock_t makeTime;
};

class ItemManager {
  ItemInfo itemList[3];
  int rndWeight;
  MapData *map;
public:
  ItemManager(MapData *map);
  void update(clock_t t);
  void makeItem(clock_t t);
  void delItem(clock_t t);
};
#endif
