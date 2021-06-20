// // @author 추헌준(20203155)

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "ItemManager.hpp"
#include "GateManager.hpp"
#include "SnakeManager.hpp"


class GameManager {
  bool running = true;
  time_t time_started;
  time_t t;
  ItemManager *item;
  GateManager *gate;
  Snake *snake;

  void load_level(int levelID);
  void render_scoreboard();
  void render_game();
public:
  GameManager();
	~GameManager();

  void showStartScreen();
  void showResultScreen();
  void update();
  void render();
  bool isRunning();
  bool atMissionSuccess();
  void loadNextLevel();

};
#endif
