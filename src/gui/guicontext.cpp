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

void GUIContext::keyDownListener(const SDL_Keycode key) {
  if (focusBuffer.empty()) {
    return;
  }

  auto selectedFrame{focusBuffer.back().second};

  switch (key) {
  case SDLK_RETURN:
    focusBuffer.back().second = descendSingleChildren(selectedFrame);
    break;

  case SDLK_BACKSPACE:
    focusBuffer.back().second = ascendSingleParents(selectedFrame);
    return;

  case SDLK_TAB:
    rotateFocus();
    break;

  case SDLK_LEFT: {
    if (selectedFrame->getParent() &&
        selectedFrame->getParent()->getLayout() != GUILayout::VERTICAL) {
      auto nextSelected{
          selectedFrame->getParent()->getPreviousChild(selectedFrame->getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }

  case SDLK_RIGHT: {
    if (selectedFrame->getParent() &&
        selectedFrame->getParent()->getLayout() != GUILayout::VERTICAL) {
      auto nextSelected{
          selectedFrame->getParent()->getNextChild(selectedFrame->getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }

  case SDLK_UP: {
    if (selectedFrame->getParent() &&
        selectedFrame->getParent()->getLayout() == GUILayout::VERTICAL) {
      auto nextSelected{
          selectedFrame->getParent()->getPreviousChild(selectedFrame->getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }

  case SDLK_DOWN: {
    if (selectedFrame->getParent() &&
        selectedFrame->getParent()->getLayout() == GUILayout::VERTICAL) {
      auto nextSelected{
          selectedFrame->getParent()->getNextChild(selectedFrame->getId())};
      focusBuffer.back().second = nextSelected;
    }
    break;
  }
  }

  for (auto component{selectedFrame}; component;
       component = component->getParent()) {
    component->keyDownListener(key);
  }
}

void GUIContext::addKeyListener(const std::string &id, const SDL_Keycode key,
                                std::function<void()> listener) {
  auto component{lookup.find(id)};

  if (component != lookup.end()) {
    component->second->addKeyListener(key, listener);
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
  lookup.erase(id);

  std::erase_if(focusBuffer,
                [&id](std::pair<GUIComponent *, GUIComponent *> focussedPair) {
                  return focussedPair.second->getId() == id;
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
          {component, descendSingleChildren(component)});
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
  std::rotate(focusBuffer.begin(),
              down ? focusBuffer.begin() + 1 : focusBuffer.end() - 1,
              focusBuffer.end());
}

void GUIContext::update() {
  auto action{[](GUIComponent &node) {
    if (!node.isVisible()) {
      return false;
    }

    node.update();

    return true;
  }};

  for (auto &[focussed, _] : focusBuffer) {
    GUITreeWalker::traverse(*focussed, action);
  }
}

void GUIContext::updateLayout() {
  auto action{[](GUIComponent &node) {
    if (!node.isVisible()) {
      return false;
    }

    node.updateLayout();

    return true;
  }};

  for (auto &[focussed, _] : focusBuffer) {
    GUITreeWalker::traverse(*focussed, action);
  }
}

void GUIContext::drawGUI() {
  for (auto &[focussed, selected] : focusBuffer) {
    guiView.drawGUIComponent(
        *focussed,
        focusBuffer.empty() ? "" : focusBuffer.back().second->getId());
  }
}

GUIComponent *GUIContext::descendSingleChildren(GUIComponent *component) {
  GUIComponent *descendant{component};

  for (; descendant->numberOfChildren() == 1;
       descendant = descendant->getNextChild(""))
    ;

  auto firstChild{descendant->getNextChild("")};

  return firstChild ? firstChild : descendant;
}

GUIComponent *GUIContext::ascendSingleParents(GUIComponent *component) {
  if (!component->getParent() || !component->getParent()->getParent()) {
    return component;
  }

  GUIComponent *predecessor{component->getParent()};

  for (; predecessor->getParent() && predecessor->getParent()->getParent() &&
         predecessor->getParent()->numberOfChildren() == 1;
       predecessor = predecessor->getParent()) {
  };

  return predecessor;
}
