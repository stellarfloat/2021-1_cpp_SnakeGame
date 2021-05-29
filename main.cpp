#include <iostream>
#include <ncurses.h>
#include <time.h>
#include <deque>
#include <time.h>
#include <unistd.h>

using namespace std;

int kbhit(void)
{
  struct timeval tv;
  fd_set read_fd;

  /* Do not wait at all, not even a microsecond */
  tv.tv_sec=0;
  tv.tv_usec=0;

  /* Must be done first to initialize read_fd */
  FD_ZERO(&read_fd);

  /* Makes select() ask if input is ready:
   * 0 is the file descriptor for stdin    */
  FD_SET(0,&read_fd);

  /* The first parameter is the number of the
   * largest file descriptor to check + 1. */
  if(select(1, &read_fd,NULL, /*No writes*/NULL, /*No exceptions*/&tv) == -1)
    return 0;  /* An error occured */

  /*  read_fd now holds a bit map of files that are
   * readable. We test the entry for the standard
   * input (file 0). */
  
if(FD_ISSET(0,&read_fd))
    /* Character pending on stdin */
    return 1;

  /* no characters were pending */
  return 0;
}

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
    curs_set(0);
    init_pair(1, COLOR_RED, COLOR_WHITE);
    box(stdscr, 0, 0);
    attron(COLOR_PAIR(1));
    wbkgd(stdscr, COLOR_PAIR(1));
    int row, col;
    getmaxyx(stdscr, row, col);
    clock_t begin, end;

    // 기본윈도우내의서브윈도우생성
    WINDOW *win = subwin(stdscr, row, col, 0, 0);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    box(win, 0, 0);
    attron(COLOR_PAIR(2));
    wbkgd(win, COLOR_PAIR(2));
    refresh();

    Snake user = Snake(win);
    user.print(win);

    int ch;
    while(1) {
        sleep(1);
        if(kbhit()) {
            user.setDir();
        }
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