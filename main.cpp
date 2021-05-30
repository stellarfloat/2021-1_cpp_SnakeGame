#include <iostream>
#include <fstream>
#include <locale.h>
#include <curses.h>
#include <ncurses.h>
#include <deque>
#include <time.h>
#include <unistd.h>
#include "kbhit.h"

using namespace std;

#define WIDTH 21
#define HEIGHT 21

int map_data[HEIGHT][WIDTH]; // temporary global map data

// const char* map_chars[] = { 
//     "\u3000", // "　"
//     "\u2B1B", // "⬛"
//     "\u2B1C", // "⬜"
//     "\u3267", // "㉧"
//     "\u26AB", // "⚫"
//     "\uFF1F"  // "？" 
// };

const wchar_t* map_chars[] = {
    L"\u3000", // "　"
    L"\u2B1B", // "⬛"
    L"\u2B1C", // "⬜"
    L"\u3267", // "㉧"
    L"\u26AB", // "⚫"
    L"\uFF1F"  // "？"
};

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
        for (int i = 0; i < WIDTH; i++)
            map_data[h][i] = (temp[i] - '0');
        h++;
    }
}

void print_map() {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // switch (map_data[i][j]) {
            //     case 0:
            //         mvaddstr(i, j, map_chars[0]); // '　'
            //         break;
            //     case 1:
            //         mvaddstr(i, j, map_chars[1]); // '⬛'
            //         break;
            //     case 2:
            //         mvaddstr(i, j, map_chars[2]); // '⬜'
            //         break;
            //     case 3:
            //         mvaddstr(i, j, map_chars[3]); // '㉧'
            //         break;
            //     case 4:
            //         mvaddstr(i, j, map_chars[4]); // '⚫'
            //         break;
            //     default:
            //         mvaddstr(i, j, map_chars[5]); // '？', error
            //         break;
            // }
            switch(map_data[i][j]) {
            case 0:
                mvaddstr(i, j, " ");
                break;
            case 1:
                mvaddstr(i, j, "#");
                break;
            case 2:
                mvaddstr(i, j, "$");
                break;
            case 3:
                mvaddstr(i, j, "*");
                break;
            case 4:
                mvaddstr(i, j, "@");
                break;
            default:
                mvaddstr(i, j, "?");
                break;
            }
        }
    }
    refresh();
}

class Snake {
public:
    bool gameEnd = false;
    char dir;
    int size, headRow, headCol;
    deque<pair<int, int>> body;

    Snake(int row=10, int col=9) {
        dir = 'l';
        headRow = row;
        headCol = col;
        size = 5;
        for(int i = 0; i < size ; i++)
            body.push_back(make_pair(headRow, headCol + i));
    }

    bool isEnd() { return gameEnd; }

    void setDir() {
        int key = getch();
        switch(key) {
            case KEY_LEFT:
                if(dir != 'r') dir = 'l';
                else gameEnd = true;
                break;
            case KEY_RIGHT:
                if(dir != 'l') dir = 'r';
                else gameEnd = true;
                break;
            case KEY_UP:
                if(dir != 'd') dir = 'u';
                else gameEnd = true;
                break;
            case KEY_DOWN:
                if(dir != 'u') dir = 'd';
                else gameEnd = true;
                break;  
        }        
        return;
    }

    void update() {
        if(dir == 'l') body.insert(body.begin(), make_pair(body[0].first, body[0].second - 1));
        else if(dir == 'r') body.insert(body.begin(), make_pair(body[0].first, body[0].second + 1));
        else if(dir == 'u') body.insert(body.begin(), make_pair(body[0].first - 1, body[0].second));
        else if(dir == 'd') body.insert(body.begin(), make_pair(body[0].first + 1, body[0].second));

        if(map_data[body.front().first][body.front().second] != 0) {
            gameEnd = true;
            return;
        }
        map_data[body.front().first][body.front().second] = 3;
        for(int i = 1; i < body.size() - 1; i++)
            map_data[body[i].first][body[i].second] = 4;
        map_data[body.back().first][body.back().second] = 0;
        body.pop_back();

    }

};

int main() {
    setlocale(LC_ALL, "");

    // load map data
    load_level();
   
    // make window
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    int row, col;
    getmaxyx(stdscr, row, col);

    // make user
    Snake user = Snake();

    while(1) {
        print_map();
        sleep(1);
        if(kbhit()) {
            user.setDir();
        }
        user.update();
        if(user.isEnd())
            break;
    }

    getch();
    endwin();
    return 0;
}