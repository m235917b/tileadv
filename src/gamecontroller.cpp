#include "gamecontroller.hpp"
#include "utils/csvreader.hpp"

GameController::GameController(View &view, GUIController &guiController)
    : running(false), characterMenu(false),
      chunk(csvToChunk("world/chunk1.wrld")), view(view),
      guiController(guiController), actors(), characters() {
  std::unique_ptr<Character> player = std::make_unique<Character>(50, 30);
  this->player = player.get();
  characters.push_back(this->player);
  actors.push_back(std::move(player));

  guiController.initGameMenus();
}

int GameController::run() {
  view.drawGame(chunk, characters, *player);

  return 0;
}

void GameController::keyDownListener(SDL_Keycode key) {
  if (running) {
    if (!characterMenu) {
      switch (key) {
      case SDLK_UP:
        player->move(0, -1, chunk);
        break;

      case SDLK_DOWN:
        player->move(0, 1, chunk);
        break;

      case SDLK_LEFT:
        player->move(-1, 0, chunk);
        break;

      case SDLK_RIGHT:
        player->move(1, 0, chunk);
        break;

      default:
        break;
      }
    }

    switch (key) {
    case SDLK_C:
      showCharacterMenu(!characterMenu);
      break;
    }
  }
}

void GameController::setRunning(const bool running) { this->running = running; }

void GameController::showCharacterMenu(const bool visible) {
  guiController.setCharacterMenuVisible(visible);
  characterMenu = visible;
}
