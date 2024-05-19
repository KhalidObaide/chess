#include "../lib.h"

GameStatusIndicator::GameStatusIndicator(const int nCS,
                                         GameManager &nGameManager)
    : GameObject({110, nCS * 8 + 5}, {40, 40}, {0, 0, 0, 0}, false),
      gameManager(nGameManager) {
  isVisible = false;
}

void GameStatusIndicator::onStatusChange(GameStatus nStatus) {
  status = nStatus;
  if (status == RUNNING) {
    isUsingTexture = false;
    isVisible = false;
    return;
  }

  isUsingTexture = true;
  isVisible = true;
  switch (status) {
  case DRAW:
    setTexture("assets/draw.bmp", gameManager.gameEngine->renderer);
    break;
  case CHECKMATE: {
    std::string filename = "assets/";
    filename += (gameManager.gameTurn == WHITE_SIDE ? "white" : "black");
    filename += "_win.bmp";
    setTexture(filename, gameManager.gameEngine->renderer);
    break;
  }
  default:
    isUsingTexture = false;
    break;
  }
}

void GameStatusIndicator::update(
    std::unordered_map<InputEventType, int> & /*events*/) {
  if (status != gameManager.status) {
    onStatusChange(gameManager.status);
  }
}
