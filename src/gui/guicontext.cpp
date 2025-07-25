#include "gui/guicontext.hpp"
#include "gui/asciiatlas.hpp"
#include "gui/guitreewalker.hpp"
#include "utils/rendercontext.hpp"

GUIContext::GUIContext(const RenderContext &renderContext)
    : guiView(renderContext), components() {}

bool GUIContext::init() { return guiView.init(); }

void GUIContext::keyDownListener(const SDL_Keycode key) {
  auto stop{false};

  auto recurse{[key](const GUIComponent &component) {
    return component.isVisible() && component.isSelected() &&
           (component.isActive() || key != SDLK_RETURN);
  }};

  auto action{[key](GUIComponent &component) {
    if (!component.isVisible() || !component.isSelected()) {
      return;
    }

    if (!component.isActive() && key == SDLK_RETURN) {
      component.setActive(true);
      return;
    }

    component.keyDownListener(key);

    // Container navigation

    if (!component.isActive()) {
      return;
    }

    const auto &selectedChild = component.getSelectedChild();

    if (selectedChild && selectedChild->isActive()) {
      return;
    }

    if ((component.getLayout() == GUILayout::VERTICAL && key == SDLK_DOWN) ||
        (component.getLayout() != GUILayout::VERTICAL && key == SDLK_RIGHT)) {
      component.selectNextChild();
    }

    if ((component.getLayout() == GUILayout::VERTICAL && key == SDLK_UP) ||
        (component.getLayout() != GUILayout::VERTICAL && key == SDLK_LEFT)) {
      component.selectPreviousChild();
    }

    if (key == SDLK_BACKSPACE) {
      component.setActive(false);
    }
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, recurse, stop);
  }
}

void GUIContext::addKeyListener(const std::string &id, const SDL_Keycode key,
                                std::function<void()> listener) {
  auto stop{false};

  auto action{[&stop, &id, key, listener](GUIComponent &component) {
    if (component.getId() == id) {
      component.addKeyListener(key, listener);

      stop = true;
    }
  }};

  auto recurse{[](const GUIComponent &) { return true; }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, recurse, stop);
  }
}

void GUIContext::addComponent(std::unique_ptr<GUIComponent> component) {
  components.push_back(std::move(component));
}

bool GUIContext::removeComponent(const std::string &id) {
  return std::erase_if(components,
                       [&id](const std::unique_ptr<GUIComponent> &component) {
                         return component->getId() == id;
                       });
}

void GUIContext::setComponentVisible(const std::string &id,
                                     const bool visible) {
  auto stop{false};

  auto action{[&stop, &id, visible](GUIComponent &component) {
    if (component.getId() == id) {
      component.setVisible(visible);
      component.setActive(visible);
      component.setSelected(visible);

      stop = true;
    }
  }};

  auto recurse{[](const GUIComponent &) { return true; }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, recurse, stop);
  }
}

void GUIContext::update() {
  auto stop{false};

  auto action{[](GUIComponent &component) {
    if (!component.isVisible()) {
      return;
    }

    component.update();
  }};

  auto recurse{
      [](const GUIComponent &component) { return component.isVisible(); }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, recurse, stop);
  }
}

void GUIContext::updateLayout() {
  auto stop{false};

  auto action{[](GUIComponent &component) {
    if (!component.isVisible()) {
      return;
    }

    component.updateLayout();
  }};

  auto recurse{
      [](const GUIComponent &component) { return component.isVisible(); }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, recurse, stop);
  }
}

void GUIContext::drawGUI() {
  auto action{[this](GUIComponent &component) {
    if (component.isVisible()) {
      this->guiView.drawGUIComponent(component);
    }
  }};

  auto stop{false};

  auto recurse{
      [](const GUIComponent &component) { return component.isVisible(); }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, recurse, stop);
  }
}
