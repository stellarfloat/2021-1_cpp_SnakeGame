// @author 한윤석(20203159)

#include <ncurses.h>

#include "SnakeManager.hpp"


Snake::Snake(MapData *map, int row, int col) {
  this->map = map;

  dir = 0;
  headRow = row;
  headCol = col;
  // headRow = 16;
  // headCol = 20;
  size = 5;
  for (int i = 0; i < size; i++)
    body.push_back(std::make_pair(headRow, headCol + i));
}

void Snake::setDir() {
  int key = getch();
  switch (key) {
  case KEY_LEFT:
    if (dir != 2) dir = 0;
    else dead = true;
    break;
  case KEY_RIGHT:
    if (dir != 0) dir = 2;
    else dead = true;
    break;
  case KEY_UP:
    if (dir != 3) dir = 1;
    else dead = true;
    break;
  case KEY_DOWN:
    if (dir != 1) dir = 3;
    else dead = true;
    break;
  }
  return;
}

void Snake::update(GateManager& wall) {
  bool getGROWTH = false;
  bool getPOSION = false;
  body.push_front(std::make_pair(body[0].first + axisID[dir].first, body[0].second + axisID[dir].second));
  int row = body.front().first, col = body.front().second;
  int map_data_id = map->getData(row, col);

  // 충돌에 의한 게임 종료
  if(map_data_id == 1 || map_data_id == 4) {
      dead = true;
      return;
  }
  // Gate 사용
  if(map_data_id == 7) {
      gateCount++;
      inGateAxis = std::make_pair(row, col);
      body.pop_front();
      wall.setGateUsage(true);
      int a = wall.gateIdx.first, b = wall.gateIdx.second;
      std::pair<int, int> GateAxis = (std::make_pair(row, col) == wall.wallList[a]) ? wall.wallList[b] : wall.wallList[a];
      int outDir = dir;
      for(int i = 0; i < 4; i++) {
          int outRow = GateAxis.first + axisID[outDir].first;
          int outCol = GateAxis.second + axisID[outDir].second;
          int map_data_id = map->getData(outRow, outCol);
          if(outRow >= 0 && outRow <= HEIGHT && outCol >= 0 && outCol <= WIDTH && map_data_id != 1 && map_data_id != 2 && map_data_id != 4) {
              this->dir = outDir;
              body.push_front(std::make_pair(outRow, outCol));
              break;
          }
          if(++outDir == 4) outDir = 0;
      } 
  }

  // 아이템 획득 판단
  row = body.front().first, col = body.front().second;
  map_data_id = map->getData(row, col);
  if(map_data_id == 5) { getGROWTH = true; itemCountGrowth++; }
  if(map_data_id == 6) { getPOSION = true; itemCountPoison++; }
  map->setData(body.front().first, body.front().second, 3);
  for(int i = 1; i < body.size() - 1; i++)
      map->setData(body[i].first, body[i].second, 4);
  map->setData(body.back().first, body.back().second, 0);
  if(!getGROWTH) body.pop_back();
  if(getPOSION) {
      map->setData(body.back().first, body.back().second, 0);
      body.pop_back();
      if(body.size() < 3) dead = true;
  }
  
  // Gate 사용 끝났는지 확인
  if(!wall.willClose && wall.getGateUsage()) {
      std::pair<int, int> tail = body.back();
      for(int i = 0; i < 4; i++) {
          std::pair<int, int> tmp = std::make_pair(tail.first + axisID[i].first, tail.second + axisID[i].second);
          if(tmp == inGateAxis) {
              wall.willClose = true;
              break;
          }
      }
  }
  else if(wall.willClose) {
      wall.willClose = false;
      wall.setGateUsage(false);
  }
}