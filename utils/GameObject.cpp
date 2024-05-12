#include "../lib.h"

GameObject::GameObject(Coordinate nPosition, Coordinate nSize, RGBA nFill) {
  position = nPosition;
  size = nSize;
  fill = nFill;
}

void GameObject::update() {}
