// @authors 추헌준(20203155), 한윤석(20203159)

#include <iostream>
#include <fstream>
#include <deque>
#include <time.h>
#include <stdlib.h>

#include <locale.h>
#include <curses.h>
#include <ncurses.h>
#include <unistd.h>


#include "GameManager.hpp"

#include <chrono>
#include <thread>


int main() {
  GameManager *game = new GameManager();

  while (game->isRunning()) {
    game->update();
    game->render();
  }
  delete game;
  //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  
  return 0;
}
