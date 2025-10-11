#include <SDL3/SDL.h>

#include "asc.hpp"

#include "utils/timer.hpp"

ApplicationStateController::ApplicationStateController()
    : currentState(ApplicationState::MAIN_MENU), graphicsManager(),
      view(graphicsManager), guiController(graphicsManager),
      gameController(view, guiController) {}

int ApplicationStateController::run() {
  int exitCode{0};

  if (graphicsManager.init() == false) {
    SDL_Log("Failed to initialize view!\n");
    return 1;
  }

  if (view.init() == false) {
    SDL_Log("Failed to initialize view!\n");
    return 2;
  }

  if (guiController.init() == false) {
    SDL_Log("Failed to initialize view!\n");
    return 3;
  }

  bool run{true};

  guiController.addKeyListener("2_play", SDLK_RETURN, [this]() {
    currentState = ApplicationState::GAMEPLAY;
    guiController.setMainMenuVisible(false);
    gameController.setRunning(true);
  });

  guiController.addKeyListener("3_exit", SDLK_RETURN,
                               [&run]() { run = false; });

  guiController.addMouseButtonListener("2_play", SDL_BUTTON_LEFT, [this]() {
    currentState = ApplicationState::GAMEPLAY;
    guiController.setMainMenuVisible(false);
    gameController.setRunning(true);
  });

  guiController.addMouseButtonListener("3_exit", SDL_BUTTON_LEFT,
                                       [&run]() { run = false; });

  guiController.setMainMenuVisible(true);

  SDL_Event e;
  SDL_zero(e);

  Timer timer{1000 / framerate};
  timer.setListener([&run, &e, this]() {
    while (SDL_PollEvent(&e) == true) {
      switch (e.type) {
      case SDL_EVENT_QUIT:
        run = false;
        break;

      case SDL_EVENT_KEY_DOWN:
        switch (currentState) {
        case ApplicationState::MAIN_MENU:
          switch (e.key.key) {
          case SDLK_ESCAPE:
            currentState = ApplicationState::GAMEPLAY;
            guiController.setMainMenuVisible(false);
            gameController.setRunning(true);
            break;

          default:
            break;
          }
          break;

        case ApplicationState::GAMEPLAY:
          switch (e.key.key) {
          case SDLK_ESCAPE:
            currentState = ApplicationState::MAIN_MENU;
            guiController.setMainMenuVisible(true);
            gameController.setRunning(false);
            break;

          default:
            break;
          }

          break;

        default:
          break;
        }

        gameController.keyDownListener(e.key.key);
        guiController.keyDownListener(e.key.key);

        break;

      case SDL_EVENT_KEY_UP:
        gameController.keyUpListener(e.key.key);
        break;

      case SDL_EVENT_MOUSE_MOTION: {
        float posX;
        float posY;
        SDL_GetMouseState(&posX, &posY);
        guiController.mouseMotionListener(posX, posY);
        break;
      }

      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        gameController.mouseDownListener(e.button.button, e.button.x,
                                         e.button.y);
        guiController.mouseButtonDownListener(
            SDL_GetMouseState(nullptr, nullptr));
        break;

      default:
        break;
      }
    }

    graphicsManager.beginFrame();

    gameController.run();
    guiController.run();

    graphicsManager.endFrame();
  });

  while (run) {
    timer.run();
  }

  graphicsManager.destroy();

  return exitCode;
}
