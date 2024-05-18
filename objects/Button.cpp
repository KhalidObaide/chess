#include "../lib.h"
#include <SDL2/SDL.h>

Button::Button(std::string nText, Coordinate nPosition, Coordinate nSize,
               std::string texturePath, SDL_Renderer *renderer)
    : GameObject(nPosition, nSize, {0, 0, 0, 0}, true) {
  text = nText;
  setTexture(texturePath, renderer);
}
