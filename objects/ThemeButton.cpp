#include "../lib.h"

ThemeButton::ThemeButton(Coordinate nPosition, Coordinate nSize,
                         ThemePicker &nThemePicker, BoardTheme nTheme)
    : Button("Theme", nPosition, nSize, "",
             nThemePicker.gameManager.gameEngine->renderer),
      themePicker(nThemePicker) {
  theme = nTheme;
  isUsingTexture = false;
  fill = theme.lightFill;
}

void ThemeButton::onMouseDown() {
  themePicker.gameManager.board.setTheme(theme);
}
