#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// types
enum File { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum Rank { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
enum Side { WHITE_SIDE = 'w', BLACK_SIDE = 'b' };
enum InputEventType {
  MOVE_MOUSE_X,
  MOVE_MOUSE_Y,
  UP_MOUSE_PRIMARY,
  DOWN_MOUSE_PRIMARY
};

enum PieceType {
  KING = 'k',
  QUEEN = 'q',
  ROOK = 'r',
  BISHOP = 'b',
  KNIGHT = 'n',
  PAWN = 'p'
};

enum EventType { GAME_TURN, MOVE_MADE };

typedef struct {
  int x;
  int y;
} Coordinate;

typedef struct {
  int r, g, b, a;
} RGBA;

typedef struct {
  File file;
  Rank rank;
} Spot;

typedef struct {
  RGBA lightFill;
  RGBA darkFill;
} BoardTheme;

typedef struct {
  EventType type;
  int value;
} Event;

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
  int zIndex;

  GameObject(Coordinate nPosition, Coordinate nSize, RGBA nFill,
             bool nIsUsingTexture);
  void setTexture(std::string path, SDL_Renderer *renderer);

  virtual void update(std::unordered_map<InputEventType, int> &events);
  virtual void onMouseDown();
  virtual void onMouseUp();
  virtual ~GameObject();
};

class GameEngine {
public:
  std::unordered_map<InputEventType, int> events;

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
  const BoardTheme theme;

public:
  Board(GameEngine *gameEngine, const int CELL_SIZE, BoardTheme nTheme);
};

class GameManager;

class Piece : public GameObject {
private:
  const int CS; // cell-size
  const double SCALE;

  Side side;
  PieceType type;
  Side gameTurn;

  GameManager &gameManager;

public:
  bool isReadyToDrag;
  bool isDragging;

  Piece(Side nSide, PieceType nType, std::string initialPosition, const int nCS,
        SDL_Renderer *renderer, GameManager &nGameManager);
  Spot convertToSpot(std::string spotString);
  void move(Spot spot);
  void getNotified(Event event);

  void update(std::unordered_map<InputEventType, int> &events) override;
  void onMouseDown() override;
  void onMouseUp() override;
};

class GameManager : public GameObject {
private:
  GameEngine *gameEngine;
  Board board;
  const int CS; // CELL_SIZE
  std::vector<Piece> pieces;
  Side gameTurn;

public:
  GameManager(GameEngine *nGameEngine, const int CELL_SIZE);
  void setGameTurn(Side nSide);
  void setBoard(
      std::vector<std::tuple<Side, PieceType, std::string>> piecePlacements);
  void update(std::unordered_map<InputEventType, int> &events) override;
  void getNotified(Event event);
};
