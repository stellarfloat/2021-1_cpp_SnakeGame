#include <chrono>
#include <thread>
#include <ncurses.h>

#include "GameManager.hpp"
#include "MapData.hpp"
#include "kbhit.hpp"


MapData *map;

GameManager::GameManager() {
  // initialize curses routines
  initscr();
  // One-character-a-time. disable the buffering of typed characters 
  // by the TTY driver and get a character-at-a-time input
  cbreak();
  // Suppress the automatic echoing of typed characters
  noecho();
  // to capture special keystrokes
  keypad(stdscr, TRUE);
  // start ncurses color
  start_color();
  // initialize color pair
  // init_pair(pair_id, foreground_color, background_color)
  init_pair(COLOR_ID_EMPTY, COLOR_BLACK, COLOR_BLACK);
  init_pair(COLOR_ID_WALL, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_ID_IWALL, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_ID_SNAKE_HEAD, COLOR_WHITE, COLOR_GREEN);
  init_pair(COLOR_ID_SNAKE_BODY, COLOR_BLACK, COLOR_GREEN);
  init_pair(COLOR_ID_ITEM_GROWTH, COLOR_WHITE, COLOR_YELLOW);
  init_pair(COLOR_ID_ITEM_POISON, COLOR_WHITE, COLOR_RED);
  init_pair(COLOR_ID_GATE, COLOR_WHITE, COLOR_MAGENTA);

  // initialize and load map data, assume cwd is 2021-1_cpp_SnakeGame/
  map = new MapData();
  map->load("snakegame/LevelData/test.txt");

  // pass the pointer to enable direct access to the map data
  item = new ItemManager(map);
  snake = new Snake(map);
  gate = new GateManager(map);

  // load map data to GateManager
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (map->getData(i, j) == 1) {
        gate->wallList.push_back(std::make_pair(i, j));
      }
    }
  }

  // save time for later use
  time_started = time(NULL);

  srand(time(NULL));
}

GameManager::~GameManager() {
  //delete snake;
  delete item;
  delete map;
  endwin();
}

void GameManager::update() {
  if (snake->isDead()) { running = false; }

  t = clock();
  if (kbhit()) { snake->setDir(); }
  item->update(t);
  gate->update(time(NULL), time_started, snake->length());
  snake->update(*gate);

  std::this_thread::sleep_for(std::chrono::milliseconds(GAMETICK_DELAY));
}

void GameManager::render() {
  //clear();
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      switch (map->getData(i, j)) {
      case 0:
        attron(COLOR_PAIR(COLOR_ID_EMPTY));
        mvaddstr(i, 2 * j, "  "); // empty space
        attroff(COLOR_PAIR(COLOR_ID_EMPTY));
        break;
      case 1:
        attron(COLOR_PAIR(COLOR_ID_WALL));
        mvaddstr(i, 2 * j, "  "); // wall
        attroff(COLOR_PAIR(COLOR_ID_WALL));
        break;
      case 2:
        attron(COLOR_PAIR(COLOR_ID_IWALL));
        mvaddstr(i, 2 * j, "||"); // immune wall
        attroff(COLOR_PAIR(COLOR_ID_IWALL));
        break;
      case 3:
        attron(COLOR_PAIR(COLOR_ID_SNAKE_HEAD));
        mvaddstr(i, 2 * j, "><"); // snake head
        attroff(COLOR_PAIR(COLOR_ID_SNAKE_HEAD));
        break;
      case 4:
        attron(COLOR_PAIR(COLOR_ID_SNAKE_BODY));
        mvaddstr(i, 2 * j, "  "); // snake body
        attroff(COLOR_PAIR(COLOR_ID_SNAKE_BODY));
        break;
      case 5:
        attron(COLOR_PAIR(COLOR_ID_ITEM_GROWTH));
        mvaddstr(i, 2 * j, "++"); // growth item
        attroff(COLOR_PAIR(COLOR_ID_ITEM_GROWTH));
        break;
      case 6:
        attron(COLOR_PAIR(COLOR_ID_ITEM_POISON));
        mvaddstr(i, 2 * j, "--"); // poison item
        attroff(COLOR_PAIR(COLOR_ID_ITEM_POISON));
        break;
      case 7:
        attron(COLOR_PAIR(COLOR_ID_GATE));
        mvaddstr(i, 2 * j, "  "); // gate
        attroff(COLOR_PAIR(COLOR_ID_GATE));
        break;
      default:
        mvaddstr(i, 2 * j, "??"); // display error
        break;
      }
    }
  }
  refresh();
}

bool GameManager::isRunning() {
  return running;
}