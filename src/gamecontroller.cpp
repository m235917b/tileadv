#include <algorithm>

#include "gamecontroller.hpp"
#include "utils/csvreader.hpp"

#include "model/fireball.hpp"

GameController::GameController(View &view, GUIController &guiController)
    : running(false), characterMenu(false), moveUp(false), moveDown(false),
      moveLeft(false), moveRight(false), chunk(csvToChunk("world/chunk1.cnk")),
      view(view), guiController(guiController), actors(), tileActors(),
      player(nullptr) {
  // create player
  std::unique_ptr<Character> player =
      std::make_unique<Character>(50, 30, TileActorType::PLAYER);
  this->player = player.get();
  tileActors.push_back(std::move(player));

  // create targeting dummy
  std::unique_ptr<Character> dummy =
      std::make_unique<Character>(80, 20, TileActorType::DUMMY);
  tileActors.push_back(std::move(dummy));

  std::unique_ptr<Item> fireball = std::make_unique<Fireball>();
  const auto fireball_ptr = fireball.get();
  this->player->addItem(std::move(fireball));
  this->player->equipActionItem(fireball_ptr);

  guiController.initGameMenus(*this->player);
}

int GameController::run() {
  int dx{0};
  int dy{0};

  if (moveUp) {
    dy = -1;
  }

  if (moveDown) {
    dy = 1;
  }

  if (moveLeft) {
    dx = -1;
  }

  if (moveRight) {
    dx = 1;
  }

  player->move(dx, dy, chunk);

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

  std::erase_if(tileActors, [&](const std::unique_ptr<TileActor> &tileActor) {
    return !tileActor->isAlive();
  });

  std::sort(tileActors.begin(), tileActors.end(),
            [](const std::unique_ptr<TileActor> &a,
               const std::unique_ptr<TileActor> &b) {
              return a->getLayer() < b->getLayer();
            });

  for (auto &actor : actors) {
    actor->update();
  }

  for (auto &tileActor : tileActors) {
    tileActor->update();
    tileActor->updateInWorld(chunk, tileActors);
  }

  view.drawGame(chunk, tileActors, *player);

  return 0;
}

void GameController::mouseDownListener(const SDL_MouseButtonFlags button,
                                       const float posX, const float posY) {
  if (running && !characterMenu) {
    if (button == SDL_BUTTON_LEFT) {
      const auto [tileX, tileY] =
          view.getTileFromPixel(static_cast<int>(posX), static_cast<int>(posY));
      player->useActionItem(tileX, tileY, *this);
    }
  }
}

void GameController::keyDownListener(SDL_Keycode key) {
  if (running) {
    if (!characterMenu) {
      switch (key) {
      case SDLK_W:
        moveUp = true;
        break;

      case SDLK_S:
        moveDown = true;
        break;

      case SDLK_A:
        moveLeft = true;
        break;

      case SDLK_D:
        moveRight = true;
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

void GameController::keyUpListener(SDL_Keycode key) {
  if (running) {
    if (!characterMenu) {
      switch (key) {
      case SDLK_W:
        moveUp = false;
        break;

      case SDLK_S:
        moveDown = false;
        break;

      case SDLK_A:
        moveLeft = false;
        break;

      case SDLK_D:
        moveRight = false;
        break;

      default:
        break;
      }
    }
  }
}

void GameController::setRunning(const bool running) { this->running = running; }

void GameController::showCharacterMenu(const bool visible) {
  guiController.setCharacterMenuVisible(visible);
  characterMenu = visible;
}

void GameController::addTileActor(std::unique_ptr<TileActor> tileActor) {
  tileActors.push_back(std::move(tileActor));
}
