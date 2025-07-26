#include "gui/guicontext.hpp"
#include "gui/asciiatlas.hpp"
#include "gui/guitreewalker.hpp"
#include "utils/rendercontext.hpp"

GUIContext::GUIContext(const RenderContext &renderContext)
    : guiView(renderContext), components(), focusStack() {}

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

  if (!focusStack.empty()) {
    GUITreeWalker::traverse(*focusStack.top(), action);
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

  auto action{[&stop, &id, visible, this](GUIComponent &component) {
    if (component.getId() == id) {
      component.setVisible(visible);
      this->setComponentActive(component, visible);

      if (visible) {
        this->focusStack.push(&component);
      } else if (!focusStack.empty() && this->focusStack.top()->getId() == id) {
        this->focusStack.pop();
      }

      stop = true;

      return false;
    }

    return true;
  }};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, action, stop);
  }
}

void GUIContext::setComponentActive(const std::string &id, const bool active) {
  auto stop{false};

  for (const auto &component : components) {
    GUITreeWalker::traverse(
        *component,
        [&id, active, &stop, this](GUIComponent &component) {
          if (component.getId() == id) {
            this->setComponentActive(component, active);

            stop = true;

            return false;
          }

          return true;
        },
        stop);
  }
}

void GUIContext::setComponentActive(GUIComponent &component,
                                    const bool active) {
  component.setActive(active);
  component.setSelected(active);

  if (!active) {
    component.forEachChild([](GUIComponent &child) {
      GUITreeWalker::traverse(child, [](GUIComponent &subchild) {
        subchild.setActive(false);
        subchild.setSelected(false);

        return true;
      });
    });
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
