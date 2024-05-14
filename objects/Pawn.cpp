#include "../lib.h"
#include <SDL2/SDL.h>

Pawn::Pawn(Side nSide, std::string initialPosition, const int nCS,
           SDL_Renderer *renderer, GameManager &nGameManager)
    : Piece(nSide, PAWN, initialPosition, nCS, renderer, nGameManager) {}

std::vector<Spot> Pawn::getValidMoves() {
  std::vector<Spot> validMoves;

  // forward-one
  int topSquare = (spot.rank + (side == WHITE_SIDE ? 1 : -1));
  bool isValid = true;
  if ((side == WHITE_SIDE) ? (topSquare <= 7) : (topSquare >= 0)) {
    for (auto &piece : gameManager.pieces) {
      if (piece->spot.rank == topSquare && piece->spot.file == spot.file) {
        isValid = false;
      }
    }
    if (isValid) {
      validMoves.push_back({spot.file, (Rank)topSquare});
    }
  }

  // forward-two ( only works if forward-one is working )
  if (isValid && spot.rank == (side == WHITE_SIDE ? 1 : 6)) {
    int topTwoSquare = (spot.rank + (side == WHITE_SIDE ? 2 : -2));
    if ((side == WHITE_SIDE) ? (topTwoSquare <= 7) : (topTwoSquare >= 0)) {
      bool isDoubleValid = true;
      for (auto &piece : gameManager.pieces) {
        if (piece->spot.rank == topTwoSquare && piece->spot.file == spot.file) {
          isDoubleValid = false;
        }
      }
      if (isDoubleValid) {
        validMoves.push_back({spot.file, (Rank)topTwoSquare});
      }
    }
  }

  return validMoves;
}
