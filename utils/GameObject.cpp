#include "../lib.h"
#include <SDL2/SDL.h>

GameObject::GameObject(Coordinate nPosition, Coordinate nSize, RGBA nFill,
                       bool nIsUsingTexture) {
  position = nPosition;
  size = nSize;
  fill = nFill;
  isUsingTexture = nIsUsingTexture;
}

void GameObject::setTexture(std::string path, SDL_Renderer *renderer) {
  image = SDL_LoadBMP(path.c_str());
  texture = SDL_CreateTextureFromSurface(renderer, image);
}

void GameObject::update() {}
