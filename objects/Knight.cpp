#include "../lib.h"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_set>

Knight::Knight(Side nSide, std::string initialPosition, const int nCS,
               SDL_Renderer *renderer, GameManager &nGameManager)
    : Piece(nSide, KNIGHT, initialPosition, nCS, renderer, nGameManager) {}

std::vector<Spot> Knight::getValidMoves(bool /*checkForCapture*/) {
  std::vector<std::pair<int, int>> allSquares = {
      {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
  };
  std::unordered_set<std::string> inBoard;
  for (auto &square : allSquares) {
    int newFile = spot.file + square.first;
    int newRank = spot.rank + square.second;
    if ((newFile >= 0 && newFile <= 7) && (newRank >= 0 && newRank <= 7)) {
      inBoard.insert(std::to_string(newFile) + std::to_string(newRank));
    }
  }

  std::unordered_set<std::string> toSkip;
  for (auto &piece : gameManager.pieces) {
    std::string spotString =
        std::to_string(piece->spot.file) + std::to_string(piece->spot.rank);
    if (piece->side == side && inBoard.find(spotString) != inBoard.end()) {
      toSkip.insert(spotString);
    }
  }

  std::vector<Spot> validMoves;
  for (std::string spotString : inBoard) {
    if (toSkip.find(spotString) == toSkip.end()) {
      validMoves.push_back(
          {(File)(spotString[0] - '0'), (Rank)(spotString[1] - '0')});
    }
  }

  return validMoves;
}
