// @author 한윤석(20203159)

#include <ctime>

#include "GateManager.hpp"


GateManager::GateManager(MapData *map) {
  this->map = map;
  wallList.clear();
  onManager = false; 
  usingGate = false; 
  open = false; 
  gateIdx = std::make_pair(-1, -1); 
  delTime = time(NULL);
}

void GateManager::managerOn(int sz, time_t time_started) {
  if (sz >= 7 || (double)(time(NULL) - time_started) >= 20)
    onManager = true;
}

void GateManager::makeGate(time_t t) {
  double r = (double)t - this->delTime;
  if (r < (double)(rand() % 10)) return;
  this->open = true;
  int a = -1, b = -1;
  while (1) {
    a = rand() % wallList.size();
    b = rand() % wallList.size();
    if (a != b && a >= 0 && a < wallList.size() && b >= 0 && b < wallList.size()) break;
  }
  gateIdx = std::make_pair(a, b);
  map->setData(wallList[a].first, wallList[a].second, 7);
  map->setData(wallList[b].first, wallList[b].second, 7);
  this->makeTime = t;
}

void GateManager::delGate(time_t t) {
  double r = (double)t - this->makeTime;
  if (r < 10.) return;
  this->open = false;
  int a = this->gateIdx.first, b = this->gateIdx.second;
  map->setData(wallList[a].first, wallList[a].second, 1);
  map->setData(wallList[b].first, wallList[b].second, 1);
  this->gateIdx = std::make_pair(-1, -1);
  this->delTime = t;
}

void GateManager::update(time_t t, time_t time_started, size_t snake_length) {
  if (!(this->onManager)) { 
    this->managerOn(snake_length, time_started); 
  } else {
    if (this->open && !this->usingGate) this->delGate(t);
    if (!this->open) this->makeGate(t);
  }
}
