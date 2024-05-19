#include "../lib.h"

FlipButton::FlipButton(const int CS, GameManager *nGameManager)
    : Button("Restart", {60, (CS * 8) + 10}, {30, 30}, "assets/flip.bmp",
             nGameManager->gameEngine->renderer),
      gameManager(nGameManager) {}

void FlipButton::onMouseDown() { gameManager->flip(); }
