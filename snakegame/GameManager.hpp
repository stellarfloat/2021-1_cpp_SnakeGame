// // @author 추헌준(20203155)

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "ItemManager.hpp"
#include "GateManager.hpp"
#include "SnakeManager.hpp"


class GameManager {
  bool running = true;
  time_t time_started;
  clock_t t;
  ItemManager *item;
  GateManager *gate;
  Snake *snake;
public:
  GameManager();
	~GameManager();

  void update();
  void render();
  bool isRunning();

};
#endif
