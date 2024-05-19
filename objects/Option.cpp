#include "../lib.h"
#include <iostream>
#include <sstream>

PromotionOption::PromotionOption(PieceType nType, const int nCS,
                                 GameManager *nGameManager)
    : GameObject({0, 0}, {nCS / 2, nCS / 2}, {0, 0, 0, 0}, true), CS(nCS),
      gameManager(nGameManager) {
  type = nType;
}

void PromotionOption::display(Side nSide, Spot nSpot) {
  side = nSide;
  spot = nSpot;

  // set the position
  if (gameManager->flipBoard) {
    position.x = (7 - spot.file) * CS;
    position.y = spot.rank * CS;
  } else {
    position.x = spot.file * CS;
    position.y = (7 - spot.rank) * CS;
  }

  if (type == BISHOP || type == KNIGHT) {
    position.y += CS / 2;
  }
  if (type == ROOK || type == KNIGHT) {
    position.x += CS / 2;
  }

  // set the proper texture
  std::stringstream filename;
  filename << "assets/" << static_cast<char>(side) << static_cast<char>(type)
           << ".bmp";
  setTexture(filename.str(), gameManager->gameEngine->renderer);
}

void PromotionOption::onMouseDown() {
  gameManager->confirmPromotion(type, side, spot);
}
