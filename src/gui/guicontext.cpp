#include <algorithm>

#include "gui/asciiatlas.hpp"
#include "gui/guicontext.hpp"
#include "gui/guitreewalker.hpp"
#include "utils/rendercontext.hpp"

GUIContext::GUIContext(const RenderContext &renderContext)
    : guiView(renderContext), components(), focusBuffer(), lookup() {}

bool GUIContext::init() {
  std::vector<std::string> imagePaths{};

  for (const auto &component : components) {
    GUITreeWalker::traverse(*component, [&imagePaths](GUIComponent &c) {
      if (!c.getImage().empty()) {
        imagePaths.push_back(c.getImage());
      }

      return true;
    });
  }

  return guiView.init(imagePaths);
}

void GUIContext::keyDownListener(const SDL_Keycode key) {
  if (!focusBuffer.empty()) {
    auto selectedComponent{focusBuffer.back().second};

    switch (key) {
    case SDLK_RETURN: {
      GUIComponent *child{selectedComponent};

      for (; child->numberOfChildren() == 1; child = child->getNextChild(""))
        ;

      auto firstChild{child->getNextChild("")};

      if (firstChild) {
        this->focusBuffer.back().second = firstChild;
      } else {
        this->focusBuffer.back().second = child;
      }

      break;
    }

    case SDLK_BACKSPACE:
      if (focusBuffer.back().second->getParent()->getParent()) {
        focusBuffer.back().second = focusBuffer.back().second->getParent();
      }
      return;

    case SDLK_LEFT: {
      if (selectedComponent->getParent()->getLayout() != GUILayout::VERTICAL) {
        auto nextSelected{selectedComponent->getParent()->getPreviousChild(
            selectedComponent->getId())};
        focusBuffer.back().second = nextSelected;
      }
      break;
    }

    case SDLK_RIGHT: {
      if (selectedComponent->getParent()->getLayout() != GUILayout::VERTICAL) {
        auto nextSelected{selectedComponent->getParent()->getNextChild(
            selectedComponent->getId())};
        focusBuffer.back().second = nextSelected;
      }
      break;
    }

    case SDLK_UP: {
      if (selectedComponent->getParent()->getLayout() == GUILayout::VERTICAL) {
        auto nextSelected{selectedComponent->getParent()->getPreviousChild(
            selectedComponent->getId())};
        focusBuffer.back().second = nextSelected;
      }
      break;
    }

    case SDLK_DOWN: {
      if (selectedComponent->getParent()->getLayout() == GUILayout::VERTICAL) {
        auto nextSelected{selectedComponent->getParent()->getNextChild(
            selectedComponent->getId())};
        focusBuffer.back().second = nextSelected;
      }
      break;
    }
    }

    for (const auto frame : focusBuffer) {
      for (auto component{frame.second}; component;
           component = component->getParent()) {
        component->keyDownListener(key);
      }
    }
  }
}

void GUIContext::addKeyListener(const std::string &id, const SDL_Keycode key,
                                std::function<void()> listener) {
  auto component{lookup.find(id)};

  if (component != lookup.end())
    component->second->addKeyListener(key, listener);
}

void GUIContext::addComponent(std::unique_ptr<GUIComponent> component) {
  GUITreeWalker::traverse(*component, [this](GUIComponent &child) {
    this->lookup.emplace(child.getId(), &child);
    return true;
  });

  components.push_back(std::move(component));
}

bool GUIContext::removeComponent(const std::string &id) {
  lookup.erase(id);

  std::erase_if(focusBuffer,
                [&id](std::pair<GUIComponent *, GUIComponent *> focussed) {
                  return focussed.second->getId() == id;
                });

  return std::erase_if(components,
                       [&id](const std::unique_ptr<GUIComponent> &component) {
                         return component->getId() == id;
                       });
}

void GUIContext::setComponentVisible(const std::string &id,
                                     const bool visible) {
  auto componentPair{lookup.find(id)};

  if (componentPair == lookup.end())
    return;

  auto component{componentPair->second};

  component->setVisible(visible);

  if (!visible) {
    std::erase_if(focusBuffer, [&id](const auto &pair) {
      return pair.first->getId() == id;
    });

    auto descendant{std::find_if(
        focusBuffer.begin(), focusBuffer.end(), [&component](const auto &pair) {
          return pair.first == component &&
                 component->isDescendant(pair.second->getId());
        })};

    if (descendant != focusBuffer.end())
      descendant->second = component->getParent();
  } else {
    if (!component->getParent()) {
      GUIComponent *child{component};

      for (; child->numberOfChildren() == 1; child = child->getNextChild(""))
        ;

      if (child->numberOfChildren() > 0)
        this->focusBuffer.push_back({component, child->getNextChild("")});
      else
        this->focusBuffer.push_back({component, child});
    }
  }
}

void GUIContext::selectComponent(const std::string &id) {
  auto stop{false};

  auto action{[&stop, &id, this](GUIComponent &component) {
    if (component.getId() == id) {
      GUIComponent *root{};

      for (auto c = &component; c != nullptr; c = c->getParent()) {
        root = c;
      }

      this->focusBuffer.push_back({root, &component});

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

  for (auto &component : focusBuffer) {
    GUITreeWalker::traverse(*component.first, action);
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

  for (auto &component : focusBuffer) {
    GUITreeWalker::traverse(*component.first, action);
  }
}

void GUIContext::drawGUI() {
  auto action{[this](GUIComponent &component) {
    if (!component.isVisible()) {
      return false;
    }

    const auto selected{!focusBuffer.empty() &&
                        focusBuffer.back().second->getId() ==
                            component.getId()};

    this->guiView.drawGUIComponent(component, selected);

    return true;
  }};

  for (auto &component : focusBuffer) {
    GUITreeWalker::traverse(*component.first, action);
  }
}
