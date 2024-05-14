#include "../lib.h"
#include <SDL2/SDL.h>
#include <vector>

GameEngine::GameEngine(std::string title, Coordinate windowSize) {
  events = {
      {MOVE_MOUSE_X, 0},
      {MOVE_MOUSE_Y, 0},
      {UP_MOUSE_PRIMARY, 0},
      {DOWN_MOUSE_PRIMARY, 0},
  };
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
  // Set background color
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // Sort game objects based on zIndex
  std::sort(gameObjects.begin(), gameObjects.end(),
            [](const GameObject *a, const GameObject *b) {
              return a->zIndex < b->zIndex;
            });

  for (GameObject *gameObject : gameObjects) {
    // Update game object
    gameObject->update(events);

    // Render game object
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

  // Present rendered frame
  SDL_RenderPresent(renderer);
}

void GameEngine::resetInputEvents(
    std::unordered_map<InputEventType, int> &events) {
  events[DOWN_MOUSE_PRIMARY] = 0;
  events[UP_MOUSE_PRIMARY] = 0;
}

void GameEngine::gameLoop() {
  bool running = true;
  while (running) {

    // handle events
    resetInputEvents(events);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_MOUSEMOTION:
        events[MOVE_MOUSE_X] = event.motion.x;
        events[MOVE_MOUSE_Y] = event.motion.y;
        break;
      case SDL_MOUSEBUTTONDOWN:
        events[DOWN_MOUSE_PRIMARY] = 1;
        break;
      case SDL_MOUSEBUTTONUP:
        events[UP_MOUSE_PRIMARY] = 1;
        break;
      }
    }

    // handle onClicks for each gameObject
    if (events[DOWN_MOUSE_PRIMARY] == 1 || events[UP_MOUSE_PRIMARY] == 1) {
      int mouseX = events[MOVE_MOUSE_X];
      int mouseY = events[MOVE_MOUSE_Y];
      for (auto it = gameObjects.rbegin(); it != gameObjects.rend(); ++it) {
        GameObject *gameObject = *it;
        SDL_Rect rpr = {gameObject->position.x, gameObject->position.y,
                        gameObject->size.x, gameObject->size.y};
        if (mouseX >= rpr.x && mouseX < rpr.x + rpr.w && mouseY >= rpr.y &&
            mouseY < rpr.y + rpr.h) {
          if (events[DOWN_MOUSE_PRIMARY] == 1) {
            gameObject->onMouseDown();
          } else {
            gameObject->onMouseUp();
          }
        }
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
