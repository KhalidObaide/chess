#include "../lib.h"

RestartButton::RestartButton(const int CS, GameManager *nGameManager)
    : Button("Restart", {20, (CS * 8) + 10}, {30, 30}, "assets/restart.bmp",
             nGameManager->gameEngine->renderer),
      gameManager(nGameManager) {}

void RestartButton::onMouseDown() { gameManager->reset(); }
