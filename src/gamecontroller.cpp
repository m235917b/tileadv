#include "gamecontroller.hpp"
#include "utils/csvreader.hpp"

GameController::GameController(View &view, GUIController &guiController)
    : running(false), characterMenu(false),
      chunk(csvToChunk("world/chunk1.cnk")), view(view),
      guiController(guiController), actors(), characters() {
  std::unique_ptr<Character> player = std::make_unique<Character>(50, 30);
  this->player = player.get();
  characters.push_back(this->player);
  actors.push_back(std::move(player));

  guiController.initGameMenus(*this->player);
}

int GameController::run() {
  if (player->getPosX() < 0) {
    if (chunk.getLeft() != "null") {
      chunk = csvToChunk("world/" + chunk.getLeft() + ".cnk");
      player->setPosX(chunk.getWidth() - 1);
    } else {
      player->move(1, 0, chunk);
    }
  } else if (player->getPosX() >= chunk.getWidth()) {
    if (chunk.getRight() != "null") {
      chunk = csvToChunk("world/" + chunk.getRight() + ".cnk");
      player->setPosX(0);
    } else {
      player->move(-1, 0, chunk);
    }
  }

  if (player->getPosY() < 0) {
    if (chunk.getUp() != "null") {
      chunk = csvToChunk("world/" + chunk.getUp() + ".cnk");
      player->setPosY(chunk.getHeight() - 1);
    } else {
      player->move(0, 1, chunk);
    }
  } else if (player->getPosY() >= chunk.getHeight()) {
    if (chunk.getDown() != "null") {
      chunk = csvToChunk("world/" + chunk.getDown() + ".cnk");
      player->setPosY(0);
    } else {
      player->move(0, -1, chunk);
    }
  }

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
