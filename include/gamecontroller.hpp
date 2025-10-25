#pragma once

#include <memory>
#include <vector>

#include <SDL3/SDL.h>

#include "guicontroller.hpp"
#include "model/actor.hpp"
#include "model/character.hpp"
#include "model/chunk.hpp"
#include "model/item.hpp"
#include "model/tileactor.hpp"
#include "staterunner.hpp"
#include "view/view.hpp"


class GameController : public StateRunner {
public:
  GameController(View &view, GUIController &guiController);
  GameController(const GameController &) = delete;
  GameController &operator=(const GameController &) = delete;
  GameController(GameController &&) = default;
  GameController &operator=(GameController &&) = delete;
  ~GameController() = default;

  int run() override;
  void mouseDownListener(const SDL_MouseButtonFlags button, const float posX,
                         const float posY);
  void keyDownListener(const SDL_Keycode key);
  void keyUpListener(const SDL_Keycode key);

  void setRunning(const bool running);
  void showCharacterMenu(const bool visible);

  void addTileActor(std::unique_ptr<TileActor> actor);

private:
  bool running;
  bool characterMenu;

  bool moveUp;
  bool moveDown;
  bool moveLeft;
  bool moveRight;

  Chunk chunk;

  View &view;
  GUIController &guiController;

  std::vector<std::unique_ptr<Actor>> actors;
  std::vector<std::unique_ptr<TileActor>> tileActors;
  Character *player;
};