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

#include <chrono>
#include <thread>


int main() {
  GameManager *game = new GameManager();

  while (game->isRunning()) {
    game->update();
    game->render();
  }
  delete game;
  return 0;
}
