#include "../lib.h"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

Bishop::Bishop(Side nSide, std::string initialPosition, const int nCS,
               SDL_Renderer *renderer, GameManager &nGameManager)
    : Piece(nSide, BISHOP, initialPosition, nCS, renderer, nGameManager) {}

std::vector<Spot> Bishop::getValidMoves(bool /*checkForCapture*/) {
  std::unordered_map<std::string, Side> allPieces;
  for (auto &piece : gameManager.pieces) {
    allPieces[std::to_string(piece->spot.file) +
              std::to_string(piece->spot.rank)] = piece->side;
  }

  std::vector<Spot> validMoves;

  // up-right
  Spot newSpot = {(File)(spot.file + 1), (Rank)(spot.rank + 1)};
  while (newSpot.file <= 7 && newSpot.rank <= 7) {
    std::string spotString =
        std::to_string(newSpot.file) + std::to_string(newSpot.rank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back(newSpot);
        break;
      }
    } else {
      validMoves.push_back({newSpot});
    }
    newSpot.file = (File)(newSpot.file + 1);
    newSpot.rank = (Rank)(newSpot.rank + 1);
  }

  // up-left
  newSpot = {(File)(spot.file - 1), (Rank)(spot.rank + 1)};
  while (newSpot.file >= 0 && newSpot.rank >= 0) {
    std::string spotString =
        std::to_string(newSpot.file) + std::to_string(newSpot.rank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back(newSpot);
        break;
      }
    } else {
      validMoves.push_back({newSpot});
    }
    newSpot.file = (File)(newSpot.file - 1);
    newSpot.rank = (Rank)(newSpot.rank + 1);
  }

  // down-right
  newSpot = {(File)(spot.file + 1), (Rank)(spot.rank - 1)};
  while (newSpot.file <= 7 && newSpot.rank >= 0) {
    std::string spotString =
        std::to_string(newSpot.file) + std::to_string(newSpot.rank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back(newSpot);
        break;
      }
    } else {
      validMoves.push_back({newSpot});
    }
    newSpot.file = (File)(newSpot.file + 1);
    newSpot.rank = (Rank)(newSpot.rank - 1);
  }

  // down-left
  newSpot = {(File)(spot.file - 1), (Rank)(spot.rank - 1)};
  while (newSpot.file >= 0 && newSpot.rank >= 0) {
    std::string spotString =
        std::to_string(newSpot.file) + std::to_string(newSpot.rank);

    if (allPieces.find(spotString) != allPieces.end()) {
      if (allPieces[spotString] == side) {
        break;
      } else {
        validMoves.push_back(newSpot);
        break;
      }
    } else {
      validMoves.push_back({newSpot});
    }
    newSpot.file = (File)(newSpot.file - 1);
    newSpot.rank = (Rank)(newSpot.rank - 1);
  }

  return validMoves;
}
