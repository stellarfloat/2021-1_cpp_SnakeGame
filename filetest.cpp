#include <iostream>
#include <fstream>
#include <ncurses.h>

using namespace std;

#define WIDTH 21
#define HEIGHT 21

void load_level() { //char**& map_data
  ifstream lv_data;

  lv_data.open("./LevelData/test.txt");
  
  if (!lv_data.is_open()) {
    cout << "error" << endl;
    return;
  }
  
  while (!lv_data.eof()) {
    char temp[22];
    lv_data.getline(temp, 22);
    cout << temp << endl;
  }

  lv_data.close();
  return;
}

int main() {
  ifstream lv_data;

  lv_data.open("LevelData/test.txt");
  
  if (lv_data.fail()) {
    cout << "error" << endl;
    return 1;
  }
  
  while (!lv_data.eof()) {
    char temp[25];
    lv_data.getline(temp, 25);
    cout << temp << endl;
  }

  lv_data.close();
  return 0;
}
