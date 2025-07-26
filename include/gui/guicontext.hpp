#pragma once

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <stack>


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

  void setComponentVisible(const std::string &id, const bool visible);
  void setComponentActive(const std::string& id, const bool active);

  void update();
  void updateLayout();
  void drawGUI();

private:
  GUIView guiView;

  std::vector<std::unique_ptr<GUIComponent>> components;
  std::stack<GUIComponent *> focusStack;

  void setComponentActive(GUIComponent& component, const bool active);
};