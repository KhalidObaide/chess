#include <string>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

// types
typedef struct {
  int x;
  int y;
} Coordinate;

typedef struct {
  int r, g, b, a;
} RGBA;

// engine-classes
class GameObject {
public:
  Coordinate position;
  Coordinate size;
  RGBA fill;
  GameObject(Coordinate nPosition, Coordinate nSize, RGBA nFill);
  void update();
};

class GameEngine {
private:
  struct Impl; // Forward declaration of implementation structure
  Impl *imp;   // Pointer to implementation
  void runFrame();

public:
  std::vector<GameObject *> gameObjects;
  GameEngine(std::string title, Coordinate windowSize);
  void registerGameObjects(std::vector<GameObject *> nGameObjects);
  void gameLoop();
  void end();
};

// (game)Object classes
class Board {
private:
  std::vector<std::vector<GameObject>> cells;

public:
  Board(GameEngine *gameEngine);
};
