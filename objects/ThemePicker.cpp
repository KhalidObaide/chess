#include "../lib.h"

const int WIDTH = 400;
const int PADDING = 10;

ThemePicker::ThemePicker(const int nCS, GameManager &nGameManager,
                         std::vector<BoardTheme> nThemes)
    : GameObject({(nCS * 8) - WIDTH - PADDING, nCS * 8 + PADDING}, {0, 0},
                 {0, 0, 0, 0}, false),
      gameManager(nGameManager) {
  int buttonsWidth = (WIDTH / nThemes.size()) - (PADDING / 2);
  int index = 0;
  for (auto &theme : nThemes) {
    themeButtons.push_back(std::make_unique<ThemeButton>(ThemeButton(
        {position.x + (index * buttonsWidth) + (index * (PADDING / 2)),
         position.y + (PADDING / 2)},
        {buttonsWidth, 30 - (PADDING / 2)}, *this, theme)));
    gameManager.gameEngine->registerGameObjects({themeButtons[index].get()});
    index++;
  }
}
