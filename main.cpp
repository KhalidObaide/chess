#include "lib.h"

int main() {
  const int CELL_SIZE = 75;
  GameEngine gameEngine = GameEngine("Chess", {CELL_SIZE * 8, CELL_SIZE * 8});
  GameManager gameManager = GameManager(&gameEngine, CELL_SIZE);
  gameEngine.gameLoop();
  return 0;
}
