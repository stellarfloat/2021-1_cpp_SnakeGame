#include <iostream>
#include <ncurses.h>
#include <time.h>
#include <deque>
#include <time.h>

using namespace std;

class Snake {
public:
    bool gameEnd = false;
    char dir;
    int size, headRow, headCol;
    deque<pair<int, int>> body;

    Snake(WINDOW* w) {
        dir = 'l';
        getmaxyx(w, headRow, headCol);
        headRow /= 2;
        headCol /= 2;
        size = 5;
        for(int i = 0; i <size ; i++)
            body.push_back(make_pair(headRow, headCol + i));
    }

    void print(WINDOW* w) {
        wclear(w);
        for(int i = 0; i < size; i++) {
            mvwprintw(w, body[i].first, body[i].second, "*");
        }
        wrefresh(w);
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
        body.pop_back();
    }

};

int main() {
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_RED, COLOR_WHITE);
    box(stdscr, 0, 0);
    attron(COLOR_PAIR(1));
    wbkgd(stdscr, COLOR_PAIR(1));

    clock_t begin, end;

    // 기본윈도우내의서브윈도우생성
    WINDOW *win = subwin(stdscr, 30, 180, 10, 10);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    box(win, 0, 0);
    attron(COLOR_PAIR(2));
    wbkgd(win, COLOR_PAIR(2));
    refresh();

    Snake user = Snake(win);
    user.print(win);

    int ch;
    while(1) {
        user.setDir();
        user.update();
        user.print(win);
        if(user.isEnd())
            break;
    }

    int win_row, win_col;
    getmaxyx(win, win_row, win_col);
    wclear(win);
    mvwprintw(win, win_row / 2, win_col / 2, "THE END");
    wrefresh(win);
    
    getch();
    delwin(win);
    endwin();
    return 0;
}