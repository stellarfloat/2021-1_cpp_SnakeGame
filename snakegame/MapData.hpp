// @author 추헌준(20203155)

#ifndef _MAPDATA_H_
#define _MAPDATA_H_

#include <string>

#include "GameConfig.hpp"


class MapData {

  int map_data[HEIGHT][WIDTH];

public:
  MapData();
  ~MapData();

  void load(std::string dir);              // 맵 파일 경로
  int getData(int i, int j);               // 인덱스 기반 맵데이터 접근
  void setData(int i, int j, int newData); // 인덱스 기반 맵데이터 조작
};
#endif
