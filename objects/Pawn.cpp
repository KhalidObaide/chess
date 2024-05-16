#include "../lib.h"
#include <SDL2/SDL.h>

Pawn::Pawn(Side nSide, std::string initialPosition, const int nCS,
           SDL_Renderer *renderer, GameManager &nGameManager)
    : Piece(nSide, PAWN, initialPosition, nCS, renderer, nGameManager) {}

std::vector<Spot> Pawn::getValidMoves(bool /*checkForCapture*/) {
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

  // top-right
  int rightSquare = (spot.file + (side == WHITE_SIDE ? 1 : -1));
  bool isTopRightValid = false;
  if (((side == WHITE_SIDE) ? (topSquare <= 7) : (topSquare >= 0)) &&
      ((side == WHITE_SIDE) ? (rightSquare <= 7) : (rightSquare >= 0))) {
    for (auto &piece : gameManager.pieces) {
      if (piece->side != side && piece->spot.rank == topSquare &&
          piece->spot.file == rightSquare) {
        isTopRightValid = true;
      }
    }
    if (isTopRightValid) {
      validMoves.push_back({File(rightSquare), (Rank)topSquare});
    }
  }

  // top-left
  int leftSquare = (spot.file + (side == WHITE_SIDE ? -1 : 1));
  bool isTopLeftValid = false;
  if (((side == WHITE_SIDE) ? (topSquare <= 7) : (topSquare >= 0)) &&
      ((side == WHITE_SIDE) ? (leftSquare >= 0) : (leftSquare <= 7))) {
    for (auto &piece : gameManager.pieces) {
      if (piece->side != side && piece->spot.rank == topSquare &&
          piece->spot.file == leftSquare) {
        isTopLeftValid = true;
      }
    }
    if (isTopLeftValid) {
      validMoves.push_back({File(leftSquare), (Rank)topSquare});
    }
  }

  return validMoves;
}
