#include "lib.h"

int main() {
  const int CELL_SIZE = 75;
  GameEngine gameEngine =
      GameEngine("Chess", {CELL_SIZE * 8, (CELL_SIZE * 8) + 50});
  GameManager gameManager = GameManager(&gameEngine, CELL_SIZE);
  RestartButton restart = RestartButton(CELL_SIZE, &gameManager);
  gameEngine.registerGameObjects({&restart});
  gameEngine.gameLoop();
  return 0;
}
