#include "gui/guicontext.hpp"
#include "gui/asciiatlas.hpp"
#include "gui/guitreewalker.hpp"
#include "utils/rendercontext.hpp"

GUIContext::GUIContext(const RenderContext &renderContext)
    : guiView(renderContext), components() {}

bool GUIContext::init() { return guiView.init(); }

void GUIContext::keyDownListener(const SDL_Keycode key) {
  auto action{[key](GUIComponent &component) {
    if (!component.isVisible() || !component.isSelected()) {
      return false;
    }

    component.keyDownListener(key);

    if (!component.isActive() && key == SDLK_RETURN &&
        component.getType() == GUIElementType::CONTAINER) {
      component.setActive(true);
      return false;
    }

    if (!component.isActive()) {
      return false;
    }

    // Container navigation

    const auto &selectedChild = component.getSelectedChild();

    if (selectedChild && selectedChild->isActive()) {
      return true;
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
      return false;
    }

    return true;
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action);
  }
}

void GUIContext::addKeyListener(const std::string &id, const SDL_Keycode key,
                                std::function<void()> listener) {
  auto stop{false};

  auto action{[&stop, &id, key, listener](GUIComponent &component) {
    if (component.getId() == id) {
      component.addKeyListener(key, listener);

      stop = true;

      return false;
    }

    return true;
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, stop);
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

      return false;
    }

    return true;
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, stop);
  }
}

void GUIContext::update() {
  auto action{[](GUIComponent &component) {
    if (!component.isVisible()) {
      return false;
    }

    component.update();

    return true;
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action);
  }
}

void GUIContext::updateLayout() {
  auto action{[](GUIComponent &component) {
    if (!component.isVisible()) {
      return false;
    }

    component.updateLayout();

    return true;
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action);
  }
}

void GUIContext::drawGUI() {
  auto action{[this](GUIComponent &component) {
    if (!component.isVisible()) {
      return false;
    }

    this->guiView.drawGUIComponent(component);

    return true;
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action);
  }
}
