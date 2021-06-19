#include <iostream>
#include <fstream>
#include <ncurses.h>

using namespace std;

#define WIDTH 21
#define HEIGHT 21

char map_data[HEIGHT][WIDTH]; // temporary global map data

void load_level() { //char**& map_data
  ifstream lv_data;

  lv_data.open("snakegame/LevelData/test.txt");
  
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

  // int row,col;
  // getmaxyx(stdscr,row,col);
  // mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);

  // sudo apt install libncursesw5-dev
  // 2-wide unicode characters for map display
  const wchar_t* map_chars[] = {
    L"\u3000", // "　"
    L"\u2B1B", // "⬛"
    L"\u2B1C", // "⬜"
    L"\u3267", // "㉧"
    L"\u26AB", // "⚫"
    L"\uFF1F"  // "？"
  };

  load_level();
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      switch (map_data[i][j]) {
        case '0':
          mvaddwstr(i, j, map_chars[0]); // '　'
          break;
        case '1':
          mvaddwstr(i, j, map_chars[1]); // '⬛'
          break;
        case '2':
          mvaddwstr(i, j, map_chars[2]); // '⬜'
          break;
        case '3':
          mvaddwstr(i, j, map_chars[3]); // '㉧'
          break;
        case '4':
          mvaddwstr(i, j, map_chars[4]); // '⚫'
          break;
        default:
          mvaddwstr(i, j, map_chars[5]); // '？', error
          break;
      }
    }
  }

  refresh();
  getch();
  endwin();

  return 0;
}
