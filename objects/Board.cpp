#include "../lib.h"
#include <vector>

Board::Board(GameEngine *gameEngine, const int CELL_SIZE, BoardTheme nTheme)
    : theme(nTheme) {
  const int CS = CELL_SIZE;
  // init board
  for (int i = 0; i < 8; i++) {
    cells.push_back({});
    for (int j = 0; j < 8; j++) {
      RGBA color = (i + j) % 2 ? theme.darkFill : theme.lightFill;
      GameObject cell = GameObject({j * CS, i * CS}, {CS, CS}, color, false);
      cells[i].push_back(cell);
    }
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      gameEngine->registerGameObjects({&cells[i][j]});
    }
  }
}
