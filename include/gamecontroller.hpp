#pragma once

#include <memory>
#include <vector>


#include <SDL3/SDL.h>

#include "guicontroller.hpp"
#include "model/actor.hpp"
#include "model/character.hpp"
#include "model/chunk.hpp"
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
  int runCharacterMenu();
  void keyDownListener(const SDL_Keycode key);

  void setRunning(const bool running);
  void showCharacterMenu(const bool visible);

private:
  bool running;
  bool characterMenu;

  Chunk chunk;

  View &view;
  GUIController &guiController;

  std::vector<std::unique_ptr<Actor>> actors;
  std::vector<Character *> characters;
  Character *player;
};