



class GameManager {
  bool running = true;
public:
  GameManager();
	~GameManager();

  void update();
  void render();
  bool isRunning();

};
