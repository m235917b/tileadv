#include <algorithm>

#include "gui/asciiatlas.hpp"
#include "gui/guicontext.hpp"
#include "gui/guitreewalker.hpp"
#include "utils/rendercontext.hpp"

GUIContext::GUIContext(const RenderContext &renderContext)
    : guiView(renderContext), components(), focusBuffer(), lookup() {}

bool GUIContext::init() {
  std::vector<std::string> imagePaths{};

  for (const auto &[_, component] : lookup) {
    if (!component->getImage().empty()) {
      imagePaths.push_back(component->getImage());
    }
  }

  return guiView.loadTextures(imagePaths);
}

void GUIContext::navigate(GUIComponent &component, SDL_Keycode key) {
  if (!component.isNavigable()) {
    return;
  }

  switch (key) {
  case SDLK_RETURN:
    focusBuffer.back().second = descendSingleChildren(component);
    break;

  case SDLK_BACKSPACE:
    focusBuffer.back().second = ascendSingleParents(component);
    return;

  case SDLK_TAB:
    rotateFocus();
    break;

  case SDLK_LEFT: {
    if (component.getParent() &&
        component.getParent()->getLayout() != GUILayout::VERTICAL) {
      auto nextSelected{
          component.getParent()->getPreviousChild(component.getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }

  case SDLK_RIGHT: {
    if (component.getParent() &&
        component.getParent()->getLayout() != GUILayout::VERTICAL) {
      auto nextSelected{component.getParent()->getNextChild(component.getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }

  case SDLK_UP: {
    if (component.getParent() &&
        component.getParent()->getLayout() == GUILayout::VERTICAL) {
      auto nextSelected{
          component.getParent()->getPreviousChild(component.getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }

  case SDLK_DOWN: {
    if (component.getParent() &&
        component.getParent()->getLayout() == GUILayout::VERTICAL) {
      auto nextSelected{component.getParent()->getNextChild(component.getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }
  }
}

void GUIContext::keyDownListener(const SDL_Keycode key) {
  if (focusBuffer.empty()) {
    return;
  }

  auto selectedFrame{focusBuffer.back().second};

  navigate(*selectedFrame, key);

  for (auto component{selectedFrame}; component;
       component = component->getParent()) {
    component->keyDownListener(key);
  }
}

void GUIContext::mouseMotionListener(const float posX, const float posY) {
  const auto component{guiView.getHitComponent(posX, posY)};

  if (component) {
    selectComponent(component->getId());
  }
}

void GUIContext::mouseButtonDownListener(const SDL_MouseButtonFlags button) {
  if (focusBuffer.empty() || !focusBuffer.back().second->isNavigable()) {
    return;
  }

  focusBuffer.back().second->mouseButtonDownListener(button);
}

void GUIContext::addKeyListener(const std::string &id, const SDL_Keycode key,
                                std::function<void()> listener) {
  auto component{lookup.find(id)};

  if (component != lookup.end()) {
    component->second->addKeyListener(key, listener);
  }
}

void GUIContext::addMouseButtonListener(const std::string &id,
                                        const SDL_MouseButtonFlags button,
                                        std::function<void()> listener) {
  auto component{lookup.find(id)};

  if (component != lookup.end()) {
    component->second->addMouseButtonListener(button, listener);
  }
}

void GUIContext::addComponent(std::unique_ptr<GUIComponent> component) {
  GUITreeWalker::traverse(*component, [this](GUIComponent &node) {
    this->lookup.emplace(node.getId(), &node);
    return true;
  });

  components.push_back(std::move(component));
}

bool GUIContext::removeComponent(const std::string &id) {
  const auto &component{lookup.find(id)};

  if (component == lookup.end()) {
    return false;
  }

  std::erase_if(
      focusBuffer,
      [&component](std::pair<GUIComponent *, GUIComponent *> focussedPair) {
        return component->second->isDescendant(focussedPair.second->getId());
      });

  std::erase_if(lookup, [&component](const auto &toDelete) {
    return component->second->isDescendant(toDelete.first);
  });

  return std::erase_if(components,
                       [&id](const std::unique_ptr<GUIComponent> &component) {
                         return component->getId() == id;
                       });
}

void GUIContext::setComponentVisible(const std::string &id,
                                     const bool visible) {
  auto componentPair{lookup.find(id)};

  if (componentPair == lookup.end()) {
    return;
  }

  auto component{componentPair->second};

  component->setVisible(visible);

  if (!visible) {
    std::erase_if(focusBuffer, [&id](const auto &focussedPair) {
      return focussedPair.first->getId() == id;
    });

    auto descendant{std::find_if(
        focusBuffer.begin(), focusBuffer.end(),
        [&component](const auto &focussedPair) {
          return focussedPair.first == component->getRoot() &&
                 component->isDescendant(focussedPair.second->getId());
        })};

    if (descendant != focusBuffer.end()) {
      descendant->second = component->getParent();
    }
  } else {
    if (!component->getParent()) {
      this->focusBuffer.push_back(
          {component, descendSingleChildren(*component)});
    }
  }
}

void GUIContext::selectComponent(const std::string &id) {
  auto componentPair{lookup.find(id)};

  if (componentPair == lookup.end()) {
    return;
  }

  auto component{componentPair->second};

  auto root{std::find_if(focusBuffer.begin(), focusBuffer.end(),
                         [&component](auto &focussedPair) {
                           return focussedPair.first == component->getRoot();
                         })};

  if (root != focusBuffer.end()) {
    root->second = component;
  }
}

void GUIContext::rotateFocus(const bool down) {
  auto rotate_to =
      down ? std::find_if(focusBuffer.rbegin(), focusBuffer.rend(),
                          [](const auto &selectedPair) {
                            return selectedPair.second->isNavigable();
                          })
                 .base()
           : std::find_if(focusBuffer.begin(), focusBuffer.end(),
                          [](const auto &selectedPair) {
                            return selectedPair.second->isNavigable();
                          });

  if (!down) {
    if (rotate_to != focusBuffer.end() &&
        rotate_to->second->getId() != focusBuffer.back().second->getId()) {
      ++rotate_to;
    } else {
      return;
    }
  }

  std::rotate(focusBuffer.begin(), rotate_to, focusBuffer.end());
}

void GUIContext::update() {
  for (auto &[focussed, _] : focusBuffer) {
    GUITreeWalker::traverse(*focussed, [](GUIComponent &node) {
      if (!node.isVisible()) {
        return false;
      }

      node.update();

      return true;
    });
  }
}

void GUIContext::updateLayout() {
  for (auto &[focussed, _] : focusBuffer) {
    GUITreeWalker::traverse(*focussed, [](GUIComponent &node) {
      if (!node.isVisible()) {
        return false;
      }

      node.updateLayout();

      return true;
    });
  }

  recomputeLayoutCache();
}

void GUIContext::drawGUI() {
  guiView.drawGUI(!focusBuffer.back().second->isNavigable()
                      ? ""
                      : focusBuffer.back().second->getId());
}

GUIComponent *GUIContext::descendSingleChildren(GUIComponent &component) {
  GUIComponent *descendant{&component};

  for (; descendant->numberOfChildren() == 1;
       descendant = descendant->getNextChild(""))
    ;

  auto firstChild{descendant->getNextChild("")};

  return firstChild ? firstChild : descendant;
}

GUIComponent *GUIContext::ascendSingleParents(GUIComponent &component) {
  if (!component.getParent() || !component.getParent()->getParent()) {
    return &component;
  }

  GUIComponent *predecessor{component.getParent()};

  for (; predecessor->getParent() && predecessor->getParent()->getParent() &&
         predecessor->getParent()->numberOfChildren() == 1;
       predecessor = predecessor->getParent()) {
  };

  return predecessor;
}

GUIComponent *GUIContext::getComponent(std::string id) {
  const auto &component{lookup.find(id)};

  if (component != lookup.end()) {
    return component->second;
  }

  return nullptr;
}

void GUIContext::recomputeLayoutCache() {
  guiView.allocateLayoutCache(lookup.size());

  for (auto &[focussed, _] : focusBuffer) {
    guiView.recomputeLayoutCache(*focussed);
  }
}
