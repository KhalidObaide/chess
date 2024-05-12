#include "../lib.h"
#include <SDL2/SDL.h>
#include <vector>

GameEngine::GameEngine(std::string title, Coordinate windowSize) {
  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y,
                            SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void GameEngine::registerGameObjects(std::vector<GameObject *> nGameObjects) {
  for (GameObject *gameObject : nGameObjects) {
    gameObjects.push_back(gameObject);
  }
}

void GameEngine::runFrame() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  for (GameObject *gameObject : gameObjects) {
    SDL_Rect rpr = {gameObject->position.x, gameObject->position.y,
                    gameObject->size.x, gameObject->size.y};
    if (gameObject->isUsingTexture) {
      SDL_RenderCopy(renderer, gameObject->texture, NULL, &rpr);
    } else {
      SDL_SetRenderDrawColor(renderer, gameObject->fill.r, gameObject->fill.g,
                             gameObject->fill.b, gameObject->fill.a);
      SDL_RenderFillRect(renderer, &rpr);
    }
  }
  SDL_RenderPresent(renderer);
}

void GameEngine::gameLoop() {
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
    runFrame();
    SDL_Delay(1000 / 60);
  }
  end();
}

void GameEngine::end() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
