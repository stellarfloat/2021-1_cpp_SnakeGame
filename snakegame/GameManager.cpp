// @author 추헌준(20203155)

#include <chrono>
#include <thread>
#include <string>
#include <ncurses.h>

#include "GameManager.hpp"
#include "MapData.hpp"
#include "kbhit.hpp"


MapData *map;
ItemManager *item;
Snake *snake;
GateManager *gate;
int currentLevel = 1;

void GameManager::load_level(int levelID) {
  if (levelID > 4) {
    // impl. game clear
  }

  // initialize and load map data, assume cwd is 2021-1_cpp_SnakeGame/
  map = new MapData();
  map->load(levelMetaData[levelID].first);

  // pass the pointer to enable direct access to the map data
  item = new ItemManager(map);
  snake = new Snake(map, levelMetaData[levelID].second.first, levelMetaData[levelID].second.second);
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
}

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

  curs_set(0);
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

  this->load_level(currentLevel);

  srand(time(NULL));
}

GameManager::~GameManager() {
  delete snake;
  delete item;
  delete map;
  endwin();
}

void GameManager::loadNextLevel() {
  clear();
  this->load_level(++currentLevel); // possible memory leak
}

bool GameManager::atMissionSuccess() {
  // levelMissionData
  // {bodyLength, itemGrowth, itemPoison, gateUsed}
  if (levelMissionData[currentLevel][0] <= snake->length()) {
    if (levelMissionData[currentLevel][1] <= snake->getItemCountGrowth()) {
      if (levelMissionData[currentLevel][2] <= snake->getItemCountPoison()) {
        if (levelMissionData[currentLevel][3] <= snake->getGateCount()) {
          return true;
        }
      }
    }
  }
  return false;
}

void GameManager::update() {
  if (snake->isDead()) { running = false; }
  if (this->atMissionSuccess()) { 
    this->loadNextLevel();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(GAMETICK_DELAY));
  if (kbhit()) { snake->setDir(); }
  item->update(time(NULL));
  gate->update(time(NULL), time_started, snake->length());
  snake->update(*gate);
}

void GameManager::render_scoreboard() {
  int offset_x = 2 * WIDTH;
  mvaddstr(0, offset_x + 2, "---------------------");
  mvaddstr(1, offset_x + 2, "|>>| Score Board |<<|");
  std::string tempstr = "   | B: " + std::to_string(snake->length());
  mvaddstr(2, offset_x + 2, &tempstr[0]);
  tempstr = "   | +: " + std::to_string(snake->getItemCountGrowth());
  mvaddstr(3, offset_x + 2, &tempstr[0]);
  tempstr = "   | -: " + std::to_string(snake->getItemCountPoison());
  mvaddstr(4, offset_x + 2, &tempstr[0]);
  tempstr =  "   | G: " + std::to_string(snake->getGateCount());
  mvaddstr(5, offset_x + 2, &tempstr[0]);
  for (int i = 2; i <= 5; i++) mvaddch(i, offset_x + 19, '|');

  mvaddstr(6, offset_x + 2, "---------------------");
  mvaddstr(7, offset_x + 2, "|>>|   Mission   |<<|");
  tempstr = "   | B: " + std::to_string(levelMissionData[currentLevel][0]);
  mvaddstr(8, offset_x + 2, &tempstr[0]);
  tempstr = levelMissionData[currentLevel][0] <= snake->length() ? "(v)" : "( )";
  mvaddstr(8, offset_x + 15, &tempstr[0]);
  tempstr = "   | +: " + std::to_string(levelMissionData[currentLevel][1]);
  mvaddstr(9, offset_x + 2, &tempstr[0]);
  tempstr = levelMissionData[currentLevel][1] <= snake->getItemCountGrowth() ? "(v)" : "( )";
  mvaddstr(9, offset_x + 15, &tempstr[0]);
  tempstr = "   | -: " + std::to_string(levelMissionData[currentLevel][2]);
  mvaddstr(10, offset_x + 2, &tempstr[0]);
  tempstr = levelMissionData[currentLevel][2] <= snake->getItemCountPoison() ? "(v)" : "( )";
  mvaddstr(10, offset_x + 15, &tempstr[0]);
  tempstr = "   | G: " + std::to_string(levelMissionData[currentLevel][3]);
  mvaddstr(11, offset_x + 2, &tempstr[0]);
  tempstr = levelMissionData[currentLevel][3] <= snake->getGateCount() ? "(v)" : "( )";
  mvaddstr(11, offset_x + 15, &tempstr[0]);
  for (int i = 7; i <= 11; i++) mvaddch(i, offset_x + 19, '|');
  mvaddstr(12, offset_x + 2, "---------------------");
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
  this->render_scoreboard();
  refresh();
}

bool GameManager::isRunning() {
  return running;
}