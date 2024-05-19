#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Piece;
class GameObject;
class PromotionOption;

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

typedef struct {
  Piece *piece;
  Spot start;
  Spot end;
} HistoryRecord;

typedef struct {
  std::unique_ptr<Piece> piece;
} CaptureRecord;

typedef struct {
  std::vector<HistoryRecord> records;
  std::unordered_map<int, std::unique_ptr<Piece>> capturedPieces;
  std::unordered_map<int, int> capturedPiecesIndexes;
} History;

typedef struct {
  Side side;
  Spot spot;
  bool hidden;
  std::vector<PromotionOption> options;
} PromotionBoard;

enum GameStatus { DRAW, CHECKMATE, RUNNING };

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
  void resetInputEvents(std::unordered_map<InputEventType, int> &events);
  void registerGameObjects(std::vector<GameObject *> nGameObjects);
  void deRegisterGameObject(GameObject *gameObjectToRemove);
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
public:
  const int CS; // cell-size
  const double SCALE;

  Side side;
  PieceType type;
  Side gameTurn;

  GameManager &gameManager;
  bool isReadyToDrag;
  bool isDragging;
  Spot spot;

  Piece(Side nSide, PieceType nType, std::string initialPosition, const int nCS,
        SDL_Renderer *renderer, GameManager &nGameManager);
  Spot convertToSpot(std::string spotString);
  void move(Spot nSpot);
  void getNotified(Event event);

  void update(std::unordered_map<InputEventType, int> &events) override;
  void onMouseDown() override;
  void onMouseUp() override;

  virtual std::vector<Spot> getValidMoves(bool checkForCapture = false);
};

class GameManager : public GameObject {
private:
  Board board;
  const int CS; // CELL_SIZE
  PromotionBoard promotionBoard;

public:
  GameEngine *gameEngine;
  bool promotionInProgress;
  bool flipBoard;
  Side gameTurn;
  History history;
  std::vector<std::unique_ptr<Piece>> pieces;
  GameManager(GameEngine *nGameEngine, const int CELL_SIZE);
  void setGameTurn(Side nSide);
  void setBoard(
      std::vector<std::tuple<Side, PieceType, std::string>> piecePlacements);
  std::vector<Spot> getValidCastleMoves(Side side);
  bool isSpotInCheck(Spot spot, Side side, bool includeKingChecks);
  void runCastleMove(Spot nSpot);
  void runCapture(Spot nSpot);
  void update(std::unordered_map<InputEventType, int> &events) override;
  void getNotified(Event event);
  void addToHistory(Piece *piece, Spot end);
  bool makeMove(Piece *piece, Spot newSpot, bool switchTurn = true);
  void rollbackMove();
  GameStatus getGameStatus(Side side);
  void runPromotion(Side side, Spot spot);
  void displayPromotionBoard(Side side, Spot spot);
  void confirmPromotion(PieceType type, Side side, Spot spot);
  void setStandardBoard();
  void reset();
  void flip();
};

class Pawn : public Piece {
public:
  Pawn(Side nSide, std::string initialPosition, const int nCS,
       SDL_Renderer *renderer, GameManager &nGameManager);
  std::vector<Spot> getValidMoves(bool checkForCapture = false) override;
};

class Knight : public Piece {
public:
  Knight(Side nSide, std::string initialPosition, const int nCS,
         SDL_Renderer *renderer, GameManager &nGameManager);
  std::vector<Spot> getValidMoves(bool checkForCapture = false) override;
};

class King : public Piece {
public:
  King(Side nSide, std::string initialPosition, const int nCS,
       SDL_Renderer *renderer, GameManager &nGameManager);
  std::vector<Spot> getValidMoves(bool checkForCapture = false) override;
};

class Rook : public Piece {
public:
  Rook(Side nSide, std::string initialPosition, const int nCS,
       SDL_Renderer *renderer, GameManager &nGameManager);
  std::vector<Spot> getValidMoves(bool checkForCapture = false) override;
};

class Bishop : public Piece {
public:
  Bishop(Side nSide, std::string initialPosition, const int nCS,
         SDL_Renderer *renderer, GameManager &nGameManager);
  std::vector<Spot> getValidMoves(bool checkForCapture = false) override;
};

class Queen : public Piece {
public:
  Queen(Side nSide, std::string initialPosition, const int nCS,
        SDL_Renderer *renderer, GameManager &nGameManager);
  std::vector<Spot> getValidMoves(bool checkForCapture = false) override;
};

class PromotionOption : public GameObject {
private:
  const int CS;
  GameManager *gameManager;
  Side side;
  Spot spot;
  PieceType type;

public:
  PromotionOption(PieceType nType, const int CS, GameManager *gameManager);
  void display(Side side, Spot spot);
  void onMouseDown() override;
};

class Button : public GameObject {
public:
  std::string text;
  RGBA color;
  Button(std::string nText, Coordinate nPosition, Coordinate nSize,
         std::string texturePath, SDL_Renderer *renderer);
};

class RestartButton : public Button {
private:
  GameManager *gameManager;

public:
  RestartButton(const int CS, GameManager *nGameManager);
  void onMouseDown() override;
};

class FlipButton : public Button {
private:
  GameManager *gameManager;

public:
  FlipButton(const int CS, GameManager *nGameManager);
  void onMouseDown() override;
};
