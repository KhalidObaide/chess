#include "lib.h"
#include <vector>

int main() {
  GameEngine gameEngine = GameEngine("Chess", {800, 800});
  GameObject player = GameObject({10, 10}, {20, 20}, {0, 0, 0, 255});
  GameObject enemy = GameObject({100, 10}, {20, 20}, {255, 0, 0, 255});
  std::vector<GameObject *> allGameObjects = {&player, &enemy};
  gameEngine.registerGameObjects(allGameObjects);
  gameEngine.gameLoop();
  return 0;
}
