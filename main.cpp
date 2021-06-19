#include <iostream>
#include <fstream>
#include <deque>
#include <ctime>
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
#define itemListLength 3
const pair<int, int> axisID[] = {make_pair(0, -1), make_pair(-1, 0), make_pair(0, 1), make_pair(1, 0)};

int map_data[HEIGHT][WIDTH]; // temporary global map data

class GateManager {
public:
    deque<pair<int, int>> wallList;
    int length;
    bool onManager;
    bool usingGate;
    bool willClose;
    bool open;
    pair<int, int> gateIdx;
    time_t makeTime;
    time_t delTime;

    GateManager() { wallList.clear(); onManager = false; usingGate = false; open = false; gateIdx = make_pair(-1, -1); delTime = time(NULL); }
    void managerOn(int sz, time_t t) {
        if(sz >= 7 || (double)(time(NULL) - t) >= 1)
            onManager = true;
    }
    void makeGate() {
        time_t nowTime = time(NULL);
        double r = (double) nowTime - this->delTime;
        if(r < (double)(rand() % 10)) return;
        this->open = true;
        int a = -1, b = -1;
        while(1) {
            a = rand() % wallList.size();
            b = rand() % wallList.size();
            if(a != b && a >= 0 && a < wallList.size() && b >= 0 && b < wallList.size()) break;
        }
        gateIdx = make_pair(a, b);
        map_data[wallList[a].first][wallList[a].second] = 7;
        map_data[wallList[b].first][wallList[b].second] = 7;
        this->makeTime = time(NULL);
    }
    void delGate() {
        time_t nowTime = time(NULL);
        double r = (double) nowTime - this->makeTime;
        if(r < 10.) return;
        this->open = false;
        int a = this->gateIdx.first, b = this->gateIdx.second;
        map_data[wallList[a].first][wallList[a].second] = 1;
        map_data[wallList[b].first][wallList[b].second] = 1;
        this->gateIdx = make_pair(-1, -1);
        this->delTime = time(NULL);
    }
};

void load_level(GateManager& wall) {
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
            map_data[h][i] = (temp[i] - '0');
            if(map_data[h][i] == 1)
                wall.wallList.push_back(make_pair(h, i));
        }
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
    int dir; // 0: left, 1:up, 2:right, 3.down
    int size, headRow, headCol;
    pair<int, int> inGateAxis;
    deque<pair<int, int>> body;

    Snake(int row = 10, int col = 9) {
        dir = 0;
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
            if (dir != 2) dir = 0;
            else gameEnd = true;
            break;
        case KEY_RIGHT:
            if (dir != 0) dir = 2;
            else gameEnd = true;
            break;
        case KEY_UP:
            if (dir != 3) dir = 1;
            else gameEnd = true;
            break;
        case KEY_DOWN:
            if (dir != 1) dir = 3;
            else gameEnd = true;
            break;
        }
        return;
    }

    void update(GateManager& wall) {
        bool getGROWTH = false;
        bool getPOSION = false;
        body.push_front(make_pair(body[0].first + axisID[dir].first, body[0].second + axisID[dir].second));
        int row = body.front().first, col = body.front().second;
        // 충돌에 의한 게임 종료
        if(map_data[row][col] == 1 || map_data[row][col] == 4) {
            gameEnd = true;
            return;
        }
        // Gate 사용
        if(map_data[row][col] == 7) {
            inGateAxis = make_pair(row, col);
            body.pop_front();
            wall.usingGate = true;
            int a = wall.gateIdx.first, b = wall.gateIdx.second;
            pair<int, int> GateAxis = (make_pair(row, col) == wall.wallList[a]) ? wall.wallList[b] : wall.wallList[a];
            int outDir = dir;
            for(int i = 0; i < 4; i++) {
                int outRow = GateAxis.first + axisID[outDir].first;
                int outCol = GateAxis.second + axisID[outDir].second;
                if(outRow >= 0 && outRow <= HEIGHT && outCol >= 0 && outCol <= WIDTH && map_data[outRow][outCol] != 1 && map_data[outRow][outCol] != 2 && map_data[outRow][outCol] != 4) {
                    this->dir = outDir;
                    body.push_front(make_pair(outRow, outCol));
                    break;
                }
                if(++outDir == 4) outDir = 0;
            } 
        }

        // 아이템 획득 판단
        if(map_data[body.front().first][body.front().second] == 5) getGROWTH = true;
        if(map_data[body.front().first][body.front().second] == 6) getPOSION = true;
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
        
        // Gate 사용 끝났는지 확인
        if(!wall.willClose && wall.usingGate) {
            pair<int, int> tail = body.back();
            for(int i = 0; i < 4; i++) {
                pair<int, int> tmp = make_pair(tail.first + axisID[i].first, tail.second + axisID[i].second);
                if(tmp == inGateAxis) {
                    wall.willClose = true;
                    break;
                }
            }
        }
        else if(wall.willClose) {
            wall.willClose = false;
            wall.usingGate = false;
        }
    }

};

struct ItemInfo {
    int row;
    int col;
    int info; // 0 GROWTH, 1 POISON
    bool ttl;
    bool first;
    time_t makeTime;
    
};

class ItemManager {
public:
    ItemInfo itemList[3];
    int rndWeight;

    ItemManager() { 
        for(int i = 0; i < itemListLength; i++) {
            itemList[i].col = 0;
            itemList[i].row = 0;
            itemList[i].info = 0;
            itemList[i].ttl = false;
            itemList[i].first = false;
            itemList[i].makeTime = 0;
        }
        rndWeight = 0;
    }
    void makeItem() {
        int check = rand() % (5 - rndWeight);
        if(check != 0) {
            rndWeight++;
            return;
        }
        rndWeight = 0;
        ItemInfo it;
        it.info = rand() % 2;
        while(1) {
            it.row = rand() % HEIGHT;
            it.col = rand() % WIDTH;
            if(map_data[it.row][it.col] == 0)
                break;
        }
        it.first = true;
        it.ttl = true;
        for(int i = 0; i < itemListLength; i++) {
            if(!itemList[i].ttl) {
                it.makeTime = time(NULL);
                map_data[it.row][it.col] = it.info + 5;
                itemList[i] = it;
                break;
            }
        }
    }
    void delItem() {
        for(int i = 0; i < itemListLength; i++) {
            if(itemList[i].ttl == false) continue;
            time_t nowTime = time(NULL);
            double r = (double)(nowTime - itemList[i].makeTime);
            if(map_data[itemList[i].row][itemList[i].col] == 0 || r >= 5) {
                itemList[i].ttl = false;
                map_data[itemList[i].row][itemList[i].col] = 0;
            }
        }
    }
};

int main() {
    srand(time(NULL));
    time_t gameStart = time(NULL);

    GateManager wall = GateManager();

    // load map data
    load_level(wall);

    init_display();

    keypad(stdscr, TRUE);
    curs_set(0);
    int row, col;
    getmaxyx(stdscr, row, col);

    // make user
    Snake user = Snake();

    ItemManager item = ItemManager();

    while (1) {
        print_map();
        if (user.isEnd())
            break;
        sleep_for(chrono::milliseconds(250));
        if (kbhit()) {
            user.setDir();
        }
        user.update(wall);
        item.makeItem();
        item.delItem();
        if(!wall.onManager) wall.managerOn(user.body.size(), gameStart);
        else {
            if(wall.open && !wall.usingGate) wall.delGate();
            if(!wall.open) wall.makeGate();
        }
    }
    getch();
    endwin();
    return 0;
}