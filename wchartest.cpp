#include <ncursesw/ncurses.h>
#include <locale.h>
#include <wchar.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();
    wchar_t wstr[] = { L'\u2B1C', L'\0' };
    mvaddwstr(0, 0, wstr);
    refresh();
    getch();
    endwin();
    return 0;
}
