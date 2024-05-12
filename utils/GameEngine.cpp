#include "../lib.h"
#include <SDL2/SDL.h>
#include <vector>

struct GameEngine::Impl {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

GameEngine::GameEngine(std::string title, Coordinate windowSize) {
  imp->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, windowSize.x,
                                 windowSize.y, SDL_WINDOW_SHOWN);
  imp->renderer = SDL_CreateRenderer(imp->window, -1, SDL_RENDERER_ACCELERATED);
}

void GameEngine::registerGameObjects(std::vector<GameObject *> nGameObjects) {
  for (GameObject *gameObject : nGameObjects) {
    gameObjects.push_back(gameObject);
  }
}

void GameEngine::runFrame() {
  SDL_SetRenderDrawColor(imp->renderer, 255, 255, 255, 255);
  SDL_RenderClear(imp->renderer);
  for (GameObject *gameObject : gameObjects) {
    SDL_SetRenderDrawColor(imp->renderer, gameObject->fill.r,
                           gameObject->fill.g, gameObject->fill.b,
                           gameObject->fill.a);
    SDL_Rect rpr = {gameObject->position.x, gameObject->position.y,
                    gameObject->size.x, gameObject->size.y};
    SDL_RenderFillRect(imp->renderer, &rpr);
  }
  SDL_RenderPresent(imp->renderer);
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
  SDL_DestroyRenderer(imp->renderer);
  SDL_DestroyWindow(imp->window);
  SDL_Quit();
}
