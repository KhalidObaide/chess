#include "lib.h"

int main() {
  GameEngine gameEngine = GameEngine("Chess", {800, 800});
  Board board = Board(&gameEngine);
  GameObject player = GameObject({20, 20}, {50, 50}, {255, 0, 0, 255}, true);
  player.setTexture("./assets/player.bmp", gameEngine.renderer);
  gameEngine.registerGameObjects({&player});
  gameEngine.gameLoop();
  return 0;
}
