#include <iostream>
#include <fstream>
#include <deque>
#include <time.h>
#include <stdlib.h>

#include <locale.h>
#include <curses.h>
#include <ncurses.h>
#include <unistd.h>

#include "kbhit.hpp"
#include "GameManager.hpp"
#include "GameConfig.hpp"

#include <chrono>
#include <thread>


int main() {
  GameManager *game = new GameManager();

  game->render();
  // while (game->isRunning()) {
  //   game->update();
  //   game->render();
  // }
  // delete game;
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  endwin();
  return 0;
}
