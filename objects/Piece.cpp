#include "../lib.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

Piece::Piece(Side nSide, PieceType nType, std::string initialPosition,
             const int nCS, SDL_Renderer *renderer, GameManager &nGameManager)
    : GameObject({0, 0}, {0, 0}, {0, 0, 0, 0}, true), CS(nCS), SCALE(0.8),
      gameManager(nGameManager) {
  size.x = CS * SCALE;
  size.y = CS * SCALE;

  side = nSide;
  type = nType;
  isReadyToDrag = false;
  isDragging = false;
  std::stringstream filename;
  filename << "assets/" << static_cast<char>(side) << static_cast<char>(type)
           << ".bmp";
  setTexture(filename.str(), renderer);
  move(convertToSpot(initialPosition));
}

Spot Piece::convertToSpot(std::string spotString) {
  if (spotString.length() != 2)
    std::runtime_error("Invalid spot format");

  char file = std::tolower(spotString[0]);
  char rank = spotString[1];
  std::unordered_map<char, File> filesOrder = {
      {'a', FILE_A}, {'b', FILE_B}, {'c', FILE_C}, {'d', FILE_D},
      {'e', FILE_E}, {'f', FILE_F}, {'g', FILE_G}, {'h', FILE_H}};
  std::unordered_map<char, Rank> ranksOrder = {
      {'1', RANK_1}, {'2', RANK_2}, {'3', RANK_3}, {'4', RANK_4},
      {'5', RANK_5}, {'6', RANK_6}, {'7', RANK_7}, {'8', RANK_8}};
  if (filesOrder.find(file) == filesOrder.end() ||
      ranksOrder.find(rank) == ranksOrder.end()) {
    std::runtime_error("Invalid move");
  }

  return {filesOrder[file], ranksOrder[rank]};
}

void Piece::move(Spot nSpot) {
  spot = nSpot;

  position.x = (spot.file * CS);
  position.y = (CS * 8) - ((spot.rank + 1) * CS);
  position.x += (CS - size.x) / 2;
  position.y += (CS - size.y) / 2;
}

void Piece::update(std::unordered_map<InputEventType, int> &events) {
  if (isDragging) {
    position.x = events[MOVE_MOUSE_X] - (size.x / 2);
    position.y = events[MOVE_MOUSE_Y] - (size.y / 2);
  }
}

void Piece::onMouseDown() {
  if (isReadyToDrag && !isDragging && gameTurn == side) {
    isDragging = true;
    zIndex = 10; // dragging piece always on top

    // not the best way to do it, but ...
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
  }
}

void Piece::onMouseUp() {
  if (isDragging) {
    isDragging = false;
    zIndex = 0; // revert to original zIndex
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

    // Snap to grid
    int snappedX = round((position.x + (size.x / 2)) / CS);
    int snappedY = 7 - round((position.y + (size.y / 2)) / CS);
    Spot newSpot = {(File)(snappedX), (Rank)(snappedY)};
    // find valid moves and revert to original spot if invalid move is chosen
    bool isValidMove = false;
    auto allValidMoves = getValidMoves();
    for (auto &validMove : allValidMoves) {
      if (newSpot.file == validMove.file && newSpot.rank == validMove.rank) {
        isValidMove = true;
        break;
      }
    }
    if (isValidMove) {
      // handle capture
      // check if it was a castle move ( edge-case )
      if (type == KING && abs(newSpot.file - spot.file) == 2) {
        gameManager.runCastleMove(newSpot);
      }
      gameManager.runCapture(newSpot);
      gameManager.addToHistory(spot, newSpot);
      move(newSpot);
      gameManager.getNotified({MOVE_MADE, side});
    } else {
      move(spot); // reset
    }
  }
}

void Piece::getNotified(Event event) {
  switch (event.type) {
  case GAME_TURN:
    gameTurn = (Side)event.value;
    break;
  default:
    break;
  }
}

std::vector<Spot> Piece::getValidMoves(bool /*checkForCapture*/) { return {}; }
