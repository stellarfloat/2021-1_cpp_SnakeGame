#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "ItemManager.hpp"
#include "SnakeManager.hpp"


class GameManager {
  bool running = true;
  clock_t t;
  ItemManager *item;
  Snake *snake;
public:
  GameManager();
	~GameManager();

  void update();
  void render();
  bool isRunning();

};
#endif
