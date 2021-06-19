#include <ncurses.h>

#include "SnakeManager.hpp"


Snake::Snake(MapData *map, int row, int col) {
  this->map = map;

  dir = 'l';
  headRow = row;
  headCol = col;
  size = 5;
  for (int i = 0; i < size; i++)
    body.push_back(std::make_pair(headRow, headCol + i));
}

void Snake::setDir() {
  int key = getch();
  switch (key) {
  case KEY_LEFT:
    if (dir != 'r') dir = 'l';
    else dead = true;
    break;
  case KEY_RIGHT:
    if (dir != 'l') dir = 'r';
    else dead = true;
    break;
  case KEY_UP:
    if (dir != 'd') dir = 'u';
    else dead = true;
    break;
  case KEY_DOWN:
    if (dir != 'u') dir = 'd';
    else dead = true;
    break;
  }
  return;
}

void Snake::update() {
  if (dir == 'l') body.insert(body.begin(), std::make_pair(body[0].first, body[0].second - 1));
  else if (dir == 'r') body.insert(body.begin(), std::make_pair(body[0].first, body[0].second + 1));
  else if (dir == 'u') body.insert(body.begin(), std::make_pair(body[0].first - 1, body[0].second));
  else if (dir == 'd') body.insert(body.begin(), std::make_pair(body[0].first + 1, body[0].second));

  int row = body.front().first, col = body.front().second;
  int map_data = map->getData(row, col);
  if (map_data != 0 && map_data != 5 && map_data != 6) {
    dead = true;
    return;
  }

  bool getGROWTH = false;
  bool getPOSION = false;
  if (map_data == 5) getGROWTH = true;
  if (map_data == 6) getPOSION = true;

  map->setData(body.front().first, body.front().second, 3);
  for (int i = 1; i < body.size() - 1; i++)
    map->setData(body[i].first, body[i].second, 4);
  map->setData(body.back().first, body.back().second, 0);
  if (!getGROWTH) body.pop_back();
  if (getPOSION) {
    map->setData(body.back().first, body.back().second, 0);
    body.pop_back();
    if (body.size() < 3) dead = true;
  }
}