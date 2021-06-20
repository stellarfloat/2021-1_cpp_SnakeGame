// @author 한윤석(20203159)

#include <chrono>
#include <stdlib.h>

#include "ItemManager.hpp"


ItemManager::ItemManager(MapData *map) {
  this->map = map;

  for (int i = 0; i < MAX_ITEM_SIZE; i++) {
    itemList[i].col = 0;
    itemList[i].row = 0;
    itemList[i].info = 0;
    itemList[i].ttl = false;
    itemList[i].first = false;
    itemList[i].makeTime = 0;
  }

  rndWeight = 0;
}

void ItemManager::makeItem(time_t t) {
  int check = rand() % (5 - rndWeight);
  if (check != 0) {
    rndWeight++;
    return;
  }
  rndWeight = 0;
  ItemInfo it;
  it.info = rand() % 2;
  while (1) {
    it.row = rand() % HEIGHT;
    it.col = rand() % WIDTH;
    if (map->getData(it.row, it.col) == 0)
      break;
  }
  it.first = true;
  it.ttl = true;
  it.makeTime = t;
  for (int i = 0; i < MAX_ITEM_SIZE; i++) {
    if (!itemList[i].ttl) {
      map->setData(it.row, it.col, it.info + 5);
      itemList[i] = it;
      break;
    }
  }
}

void ItemManager::delItem(time_t t) {
  for (int i = 0; i < MAX_ITEM_SIZE; i++) {
    if(itemList[i].ttl == false) continue;
    if (map->getData(itemList[i].row, itemList[i].col) == 0 || (double)(t - itemList[i].makeTime) >= 5.) {
      itemList[i].ttl = false;
      map->setData(itemList[i].row, itemList[i].col, 0);
    }
  }
}

void ItemManager::update(time_t t) {
  this->makeItem(t);
  this->delItem(t);
}