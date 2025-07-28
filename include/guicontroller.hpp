#pragma once

#include "gui/guicontext.hpp"
#include "staterunner.hpp"

class RenderContext;
struct SDL_Renderer;

class Character;

class GUIController : public StateRunner {
public:
  GUIController(const RenderContext &renderContext);

  bool init();

  void initGameMenus(const Character &player);

  void keyDownListener(const SDL_Keycode key);
  void addKeyListener(const std::string &id, const SDL_Keycode key,
                      std::function<void()> listener);

  void setMainMenuVisible(const bool visible);
  void setCharacterMenuVisible(const bool visible);

  int run() override;

private:
  GUIContext guiContext;
};
