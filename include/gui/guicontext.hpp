#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

#include "gui/guicomponent.hpp"
#include "gui/guiview.hpp"

class RenderContext;

class GUIContext {
public:
  GUIContext(const RenderContext &renderContext);

  bool init();

  void keyDownListener(const SDL_Keycode key);
  void addKeyListener(const std::string &id, const SDL_Keycode key,
                      std::function<void()> listener);

  void addComponent(std::unique_ptr<GUIComponent> component);
  bool removeComponent(const std::string &id);

  void setComponentVisible(const std::string &id, const bool visible = true);
  void selectComponent(const std::string &id);
  void rotateFocus(const bool down = false);

  void update();
  void updateLayout();
  void drawGUI();

private:
  GUIView guiView;

  std::vector<std::unique_ptr<GUIComponent>> components;
  std::vector<std::pair<GUIComponent *, GUIComponent *>> focusBuffer;

  std::unordered_map<std::string, GUIComponent *> lookup;

  GUIComponent *descendSingleChildren(GUIComponent &component);
  GUIComponent *ascendSingleParents(GUIComponent &component);

  void navigate(GUIComponent &component, SDL_Keycode key);
};