#include <iostream>
#include <fstream>
#include <deque>
#include <time.h>
#include <stdlib.h>

#include <locale.h>
#include <curses.h>
#include <ncurses.h>
#include <unistd.h>

#include "kbhit.h"

#include <chrono>
#include <thread>

using namespace std;
using this_thread::sleep_for;
#define WIDTH 21
#define HEIGHT 21

int map_data[HEIGHT][WIDTH]; // temporary global map data

void load_level() {
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

void init_display() {
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
    #define COLOR_ID_EMPTY       0
    #define COLOR_ID_WALL        1
    #define COLOR_ID_IWALL       2
    #define COLOR_ID_SNAKE_HEAD  3
    #define COLOR_ID_SNAKE_BODY  4
    #define COLOR_ID_ITEM_GROWTH 5
    #define COLOR_ID_ITEM_POISON 6
    #define COLOR_ID_GATE        7

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
}

void print_map() {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            switch (map_data[i][j]) {
            case 0:
                attron(COLOR_PAIR(COLOR_ID_EMPTY));
                mvaddstr(i, 2*j, "  "); // empty space
                attroff(COLOR_PAIR(COLOR_ID_EMPTY));
                break;
            case 1:
                attron(COLOR_PAIR(COLOR_ID_WALL));
                mvaddstr(i, 2*j, "  "); // wall
                attroff(COLOR_PAIR(COLOR_ID_WALL));
                break;
            case 2:
                attron(COLOR_PAIR(COLOR_ID_IWALL));
                mvaddstr(i, 2*j, "||"); // immune wall
                attroff(COLOR_PAIR(COLOR_ID_IWALL));
                break;
            case 3:
                attron(COLOR_PAIR(COLOR_ID_SNAKE_HEAD));
                mvaddstr(i, 2*j, "><"); // snake head
                attroff(COLOR_PAIR(COLOR_ID_SNAKE_HEAD));
                break;
            case 4:
                attron(COLOR_PAIR(COLOR_ID_SNAKE_BODY));
                mvaddstr(i, 2*j, "  "); // snake body
                attroff(COLOR_PAIR(COLOR_ID_SNAKE_BODY));
                break;
            case 5:
                attron(COLOR_PAIR(COLOR_ID_ITEM_GROWTH));
                mvaddstr(i, 2*j, "++"); // growth item
                attroff(COLOR_PAIR(COLOR_ID_ITEM_GROWTH));
                break;
            case 6:
                attron(COLOR_PAIR(COLOR_ID_ITEM_POISON));
                mvaddstr(i, 2*j, "--"); // poison item
                attroff(COLOR_PAIR(COLOR_ID_ITEM_POISON));
                break;
            case 7:
                attron(COLOR_PAIR(COLOR_ID_GATE));
                mvaddstr(i, 2*j, "  "); // gate
                attroff(COLOR_PAIR(COLOR_ID_GATE));
                break;
            default:
                mvaddstr(i, 2*j, "??"); // display error
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

    Snake(int row = 10, int col = 9) {
        dir = 'l';
        headRow = row;
        headCol = col;
        size = 5;
        for (int i = 0; i < size; i++)
            body.push_back(make_pair(headRow, headCol + i));
    }

    bool isEnd() { return gameEnd; }

    void setDir() {
        int key = getch();
        switch (key) {
        case KEY_LEFT:
            if (dir != 'r') dir = 'l';
            else gameEnd = true;
            break;
        case KEY_RIGHT:
            if (dir != 'l') dir = 'r';
            else gameEnd = true;
            break;
        case KEY_UP:
            if (dir != 'd') dir = 'u';
            else gameEnd = true;
            break;
        case KEY_DOWN:
            if (dir != 'u') dir = 'd';
            else gameEnd = true;
            break;
        }
        return;
    }

    void update() {
        if(dir == 'l') body.insert(body.begin(), make_pair(body[0].first, body[0].second - 1));
        else if (dir == 'r') body.insert(body.begin(), make_pair(body[0].first, body[0].second + 1));
        else if (dir == 'u') body.insert(body.begin(), make_pair(body[0].first - 1, body[0].second));
        else if (dir == 'd') body.insert(body.begin(), make_pair(body[0].first + 1, body[0].second));

        int row = body.front().first, col = body.front().second;
        if(map_data[row][col] != 0 && map_data[row][col] != 5 && map_data[row][col] != 6) {
            gameEnd = true;
            return;
        }

        bool getGROWTH = false;
        bool getPOSION = false;
        if(map_data[row][col] == 5) getGROWTH = true;
        if(map_data[row][col] == 6) getPOSION = true;
        
        map_data[body.front().first][body.front().second] = 3;
        for(int i = 1; i < body.size() - 1; i++)
            map_data[body[i].first][body[i].second] = 4;
        map_data[body.back().first][body.back().second] = 0;
        if(!getGROWTH) body.pop_back();
        if(getPOSION) {
            map_data[body.back().first][body.back().second] = 0;
            body.pop_back();
            if(body.size() < 3) gameEnd = true;
        }
    }

};

struct ItemInfo {
    int row;
    int col;
    int info; // 0 GROWTH, 1 POISON
    clock_t makeTime;
};

class ItemManager {
public:
    deque<ItemInfo> itemList;
    int rndWeight;

    ItemManager() { itemList.clear(); rndWeight = 0; }
    void makeItem() {
        if(itemList.size() == 3) return;
        int check = rand() % (5 - rndWeight);
        if(check != 0) {
            rndWeight++;
            return;
        }
        rndWeight = 0;
        if(itemList.size() < 3) {
            ItemInfo it;
            it.info = rand() % 2;
            while(1) {
                it.row = rand() % HEIGHT;
                it.col = rand() % WIDTH;
                if(map_data[it.row][it.col] == 0) {
                    map_data[it.row][it.col] = 5 + it.info;
                    break;
                }
            }
            it.makeTime = clock();
            itemList.push_back(it);
        }
    }
    void delItem() {
        for(int i = 0; i < itemList.size(); i++) {
            if(map_data[itemList[i].row][itemList[i].col] == 0) {
                if(i == 0) itemList.pop_front();
                if(i == 1) { itemList[1] = itemList[2]; itemList.pop_back(); }
                else itemList.pop_back();
            }
        }

        while(1) {
            ItemInfo tmp = itemList.front();
            if((double)(tmp.makeTime - clock()) / CLOCKS_PER_SEC > 0.1) {
                map_data[tmp.row][tmp.col] = 0;
                itemList.pop_front();
            } else { break; }
        }
    }
};

int main() {
    srand(time(NULL));
    
    // load map data
    load_level();

    init_display();

    keypad(stdscr, TRUE);
    curs_set(0);
    int row, col;
    getmaxyx(stdscr, row, col);

    // make user
    Snake user = Snake();

    ItemManager item = ItemManager();

    while (1) {
        item.delItem();
        item.makeItem();
        print_map();
        if (user.isEnd())
            break;
        sleep_for(chrono::milliseconds(250));
        if (kbhit()) {
            user.setDir();
        }
        user.update();
       
    }

    getch();
    endwin();
    return 0;
}