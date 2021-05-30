#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>

using namespace std;

#define WIDTH 21
#define HEIGHT 21

char map_data[HEIGHT][WIDTH]; // temporary global map data

void load_level() { //char**& map_data
  ifstream lv_data;

  lv_data.open("LevelData/test.txt");
  
  if (lv_data.fail()) {
    cerr << "loading error" << endl;
    return;
  }
  
  int h = 0;
  while (!lv_data.eof()) {
    char temp[25];
    lv_data.getline(temp, 25);
    for (int i = 0; i < WIDTH; i++) {
      map_data[h][i] = temp[i];
    }
    h++;
  }

  lv_data.close();
  return;
}

int main() {
  // initialize locale as UTF-8 in modern systems, 
  // need to be done before initscr()
  setlocale(LC_ALL, "");

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

  // definition for ncurses color pair id
  #define COLOR_ID_EMPTY      0
  #define COLOR_ID_WALL       1
  #define COLOR_ID_IWALL      2
  #define COLOR_ID_SNAKE_HEAD 3
  #define COLOR_ID_SNAKE_BODY 4

  // initialize color pair
  // init_pair(pair_id, foreground_color, background_color)
  init_pair(COLOR_ID_EMPTY, COLOR_BLACK, COLOR_BLACK);
  init_pair(COLOR_ID_WALL, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_ID_IWALL, COLOR_BLACK, COLOR_YELLOW);
  init_pair(COLOR_ID_SNAKE_HEAD, COLOR_WHITE, COLOR_GREEN);
  init_pair(COLOR_ID_SNAKE_BODY, COLOR_BLACK, COLOR_GREEN);
  
  // load level
  load_level();
  // loop map_data and display map
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      switch (map_data[i][j]) {
        case '0':
          attron(COLOR_PAIR(COLOR_ID_EMPTY));
          mvaddstr(i, 2*j, "  "); // empty space
          attroff(COLOR_PAIR(COLOR_ID_EMPTY));
          break;
        case '1':
          attron(COLOR_PAIR(COLOR_ID_WALL));
          mvaddstr(i, 2*j, "  "); // wall
          attroff(COLOR_PAIR(COLOR_ID_WALL));
          break;
        case '2':
          attron(COLOR_PAIR(COLOR_ID_IWALL));
          mvaddstr(i, 2*j, "  "); // immune wall
          attroff(COLOR_PAIR(COLOR_ID_IWALL));
          break;
        case '3':
          attron(COLOR_PAIR(COLOR_ID_SNAKE_HEAD));
          mvaddstr(i, 2*j, "<>"); // snake head
          attroff(COLOR_PAIR(COLOR_ID_SNAKE_HEAD));
          break;
        case '4':
          attron(COLOR_PAIR(COLOR_ID_SNAKE_BODY));
          mvaddstr(i, 2*j, "  "); // snake body
          attroff(COLOR_PAIR(COLOR_ID_SNAKE_BODY));
          break;
        default:
          mvaddstr(i, 2*j, "??"); // display error
          break;
      }
    }
  }

  refresh();
  getch();
  endwin();

  return 0;
}
