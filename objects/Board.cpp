#include "../lib.h"
#include <vector>

Board::Board(GameEngine *gameEngine) {
  const int CS = 100; // CELL_SIZE
  // init board
  for (int i = 0; i < 8; i++) {
    cells.push_back({});
    for (int j = 0; j < 8; j++) {
      RGBA color =
          (i + j) % 2 ? (RGBA){75, 75, 75, 255} : (RGBA){255, 255, 255, 255};
      GameObject cell = GameObject({j * CS, i * CS}, {CS, CS}, color);
      cells[i].push_back(cell);
      gameEngine->registerGameObjects({&cells[i][j]});
    }
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      gameEngine->registerGameObjects({&cells[i][j]});
    }
  }
}
