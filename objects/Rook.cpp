#include "../lib.h"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

Rook::Rook(Side nSide, std::string initialPosition, const int nCS,
           SDL_Renderer *renderer, GameManager &nGameManager)
    : Piece(nSide, ROOK, initialPosition, nCS, renderer, nGameManager) {}

std::vector<Spot> Rook::getValidMoves(bool /*checkForCapture*/) {
  std::unordered_map<std::string, Side> allPieces;
  for (auto &piece : gameManager.pieces) {
    allPieces[std::to_string(piece->spot.file) +
              std::to_string(piece->spot.rank)] = piece->side;
  }

  std::vector<Spot> validMoves;

  // up
  int newRank = spot.rank + 1;
  while (newRank <= 7) {
    std::string spotString =
        std::to_string(spot.file) + std::to_string(newRank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back({spot.file, (Rank)newRank});
        break;
      }
    } else {
      validMoves.push_back({spot.file, (Rank)newRank});
    }
    newRank++;
  }

  // down
  newRank = spot.rank + -1;
  while (newRank >= 0) {
    std::string spotString =
        std::to_string(spot.file) + std::to_string(newRank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back({spot.file, (Rank)newRank});
        break;
      }
    } else {
      validMoves.push_back({spot.file, (Rank)newRank});
    }
    newRank--;
  }

  // right
  int newFile = spot.file + 1;
  while (newFile <= 7) {
    std::string spotString =
        std::to_string(newFile) + std::to_string(spot.rank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back({(File)newFile, spot.rank});
        break;
      }
    } else {
      validMoves.push_back({(File)newFile, spot.rank});
    }
    newFile++;
  }

  // left
  newFile = spot.file - 1;
  while (newFile >= 0) {
    std::string spotString =
        std::to_string(newFile) + std::to_string(spot.rank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back({(File)newFile, spot.rank});
        break;
      }
    } else {
      validMoves.push_back({(File)newFile, spot.rank});
    }
    newFile--;
  }

  return validMoves;
}
