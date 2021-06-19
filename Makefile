all:
	g++ snakegame/main.cpp snakegame/kbhit.cpp -lncurses -o /tmp/snakegame.out && /tmp/snakegame.out