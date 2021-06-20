# 2021-1_cpp_SnakeGame

(2021-1 C++프로그래밍 프로젝트) Snake Game

> 현재 README 작성중입니다.

## 구현 단계

- [x] [#1] 1단계 - Snake Map을 Game화면으로 표시하는 프로그램을 완성
- [x] [#2] 2단계 - 1단계의 맵 위에 Snake를 표시하고, 화살표를 입력받아 Snake가 움직이도록 프로그램을 완성
- [x] [#3] 3단계 - 2단계 프로그램에서, Map위에 Growth Item와 Poison Item을 출현하도록 수정
- [x] [#4] 4단계 - 3단계 프로그램에서, Map의 Wall의 임의의 위치에 한 쌍의 Gate가 출현할 수 있도록 변경하고, 각 Gate에 Snake가 통과할 수 있도록 수정
- [x] [#5] 5단계 - 점수 표시 화면, 미션 시스템, 스테이지 시스템 구현

## 프로젝트 구조

```
2021-1_cpp_SnakeGame/
├── Makefile
├── README.md
├── experiments
│   └── DisplayMapTest.cpp
└── snakegame
    ├── GameConfig.hpp     // @author 추헌준(20203155)
    ├── GameManager.cpp    // @author 추헌준(20203155)
    ├── GameManager.hpp    // @author 추헌준(20203155)
    ├── GateManager.cpp    // @author 한윤석(20203159)
    ├── GateManager.hpp    // @authors 추헌준(20203155), 한윤석(20203159)
    ├── ItemManager.cpp    // @author 한윤석(20203159)
    ├── ItemManager.hpp    // @authors 추헌준(20203155), 한윤석(20203159)
    ├── LevelData          // @author 한윤석(20203159)
    │   ├── blank.txt
    │   ├── map1.txt
    │   ├── map2.txt
    │   ├── map3.txt
    │   └── map4.txt
    ├── MapData.cpp        // @author 추헌준(20203155)
    ├── MapData.hpp        // @author 추헌준(20203155)
    ├── SnakeManager.cpp   // @author 한윤석(20203159)
    ├── SnakeManager.hpp   // @authors 추헌준(20203155), 한윤석(20203159)
    ├── kbhit.cpp          // @author 한윤석(20203159)
    ├── kbhit.hpp          // @author 한윤석(20203159)
    └── main.cpp           // @authors 추헌준(20203155), 한윤석(20203159)
```

## 실행 방법

### Prerequisite

```
sudo apt install libncurses5-dev
```

### Download

```
git clone https://github.com/stellarfloat/2021-1_cpp_SnakeGame.git
cd 2021-1_cpp_SnakeGame
```

### Build & Run

```
make
```
