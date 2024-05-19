#include "lib.h"

std::vector<BoardTheme> THEMES_SET = {
    {{255, 255, 255, 255}, {0, 0, 0, 255}},       // Classic
    {{177, 228, 185, 255}, {112, 162, 163, 255}}, // Coral
    {{204, 183, 174, 255}, {112, 102, 119, 255}}, // Dusk
    {{157, 172, 255, 255}, {111, 115, 210, 255}}, // Marine
    {{234, 240, 206, 255}, {187, 190, 100, 255}}, // Wheat
    {{173, 189, 143, 255}, {111, 143, 114, 255}}, // Emerald
    {{227, 193, 111, 255}, {184, 139, 74, 255}},  // Sandcastle
};

int main() {
  const int CELL_SIZE = 75;
  GameEngine gameEngine =
      GameEngine("Chess", {CELL_SIZE * 8, (CELL_SIZE * 8) + 50});
  GameManager gameManager = GameManager(&gameEngine, CELL_SIZE, THEMES_SET[6]);
  RestartButton restart = RestartButton(CELL_SIZE, &gameManager);
  FlipButton flip = FlipButton(CELL_SIZE, &gameManager);
  GameStatusIndicator statusIndicator =
      GameStatusIndicator(CELL_SIZE, gameManager);
  ThemePicker themePicker = ThemePicker(CELL_SIZE, gameManager, THEMES_SET);
  gameEngine.registerGameObjects(
      {&restart, &flip, &statusIndicator, &themePicker});
  gameEngine.gameLoop();
  return 0;
}
