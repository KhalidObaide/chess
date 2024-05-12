#include <SDL2/SDL.h>
#include <string>
#include <vector>

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
private:
  SDL_Surface *image;

public:
  Coordinate position;
  Coordinate size;
  RGBA fill;
  SDL_Texture *texture;
  bool isUsingTexture;
  GameObject(Coordinate nPosition, Coordinate nSize, RGBA nFill,
             bool nIsUsingTexture);
  void setTexture(std::string path, SDL_Renderer *renderer);
  void update();
};

class GameEngine {
public:
  SDL_Renderer *renderer;
  SDL_Window *window;
  void runFrame();

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
