#include "../lib.h"

GameManager::GameManager(GameEngine *gameEngine, const int CELL_SIZE)
    : CS(CELL_SIZE), board(gameEngine) {}
