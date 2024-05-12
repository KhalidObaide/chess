#include "lib.h"

int main() {
  GameEngine gameEngine = GameEngine("Chess", {800, 800});
  Board board = Board(&gameEngine);
  gameEngine.gameLoop();
  return 0;
}
