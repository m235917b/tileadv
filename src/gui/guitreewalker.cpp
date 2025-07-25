#include "gui/guitreewalker.hpp"

void GUITreeWalker::traverse(
    const std::vector<std::unique_ptr<GUIComponent>> &components,
    std::function<void(const std::unique_ptr<GUIComponent> &component)>
        action) {
  for (const auto &component : components) {
    GUITreeWalker::traverseComponent(component, action);
  }
}

void GUITreeWalker::traverse(
    const std::vector<std::unique_ptr<GUIComponent>> &components,
    std::function<void(const std::unique_ptr<GUIComponent> &parent,
                       const std::unique_ptr<GUIComponent> &component)>
        action,
    std::function<bool(const std::unique_ptr<GUIComponent> &component)> recurse,
    bool &stop) {
  for (const auto &component : components) {
    GUITreeWalker::traverseComponent(nullptr, component, action, recurse, stop);
  }
}

void GUITreeWalker::traverseComponent(
    const std::unique_ptr<GUIComponent> &component,
    std::function<void(const std::unique_ptr<GUIComponent> &component)>
        action) {
  action(component);

  const auto &components = component->getChildren();

  for (const auto &component : components) {
    GUITreeWalker::traverseComponent(component, action);
  }
}

void GUITreeWalker::traverseComponent(
    const std::unique_ptr<GUIComponent> &component,
    std::function<void(const std::unique_ptr<GUIComponent> &parent,
                       const std::unique_ptr<GUIComponent> &component)>
        action,
    std::function<bool(const std::unique_ptr<GUIComponent> &component)> recurse,
    bool &stop) {
  GUITreeWalker::traverseComponent(nullptr, component, action, recurse, stop);
}

void GUITreeWalker::traverseComponent(
    const std::unique_ptr<GUIComponent> &parent,
    const std::unique_ptr<GUIComponent> &component,
    std::function<void(const std::unique_ptr<GUIComponent> &parent,
                       const std::unique_ptr<GUIComponent> &component)>
        action,
    std::function<bool(const std::unique_ptr<GUIComponent> &component)> recurse,
    bool &stop) {
  if (stop) {
    return;
  }

  action(parent, component);

  if (recurse(component)) {
    for (const auto &component : component->getChildren()) {
      GUITreeWalker::traverseComponent(parent, component, action, recurse,
                                       stop);
    }
  }
}
