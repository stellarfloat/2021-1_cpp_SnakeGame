// @authors 추헌준(20203155), 한윤석(20203159)

#ifndef _GATEMANAGER_H_
#define _GATEMANAGER_H_

#include <ctime>
#include <deque>

#include "MapData.hpp"


class GateManager {
  MapData *map;

  int length;
  bool onManager;
  bool usingGate;
  bool open;
  time_t makeTime;
  time_t delTime;
public:
  bool willClose;
  std::deque<std::pair<int, int>> wallList;
  std::pair<int, int> gateIdx;

  GateManager(MapData *map);

  void managerOn(int sz, time_t time_started);
  void update(time_t t, time_t time_started, size_t snake_length);
  void makeGate(time_t t);
  void delGate(time_t t);

  void setGateUsage(bool val) { usingGate = val; }
  bool getGateUsage() { return usingGate; }
};
#endif
