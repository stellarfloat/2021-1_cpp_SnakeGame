// @author 추헌준(20203155)

#include <fstream>
#include <iostream>

#include "MapData.hpp"
#include "GateManager.hpp"


MapData::MapData() {

}

MapData::~MapData() {

}

void MapData::load(std::string dir) {
  std::ifstream lv_data;

  lv_data.open(dir);

  if (lv_data.fail()) {
    std::cerr << "loading error" << std::endl;
    return;
  }

  int h = 0;
  while (h < HEIGHT) {// !lv_data.eof()
    char temp[100];
    lv_data.getline(temp, 100);
    for (int i = 0; i < WIDTH; i++)
      map_data[h][i] = (temp[i] - '0');
    h++;
  }
}

int MapData::getData(int i, int j) {
  return map_data[i][j];
}

void MapData::setData(int i, int j, int newData) {
  if (newData >= 0 && newData <= 7) { // 맵 요소는 0~7만 존재 
    map_data[i][j] = newData;
  }
}
