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
    bool &stop, bool &recurse) {
  for (const auto &component : components) {
    GUITreeWalker::traverseComponent(nullptr, component, action, stop, recurse);
  }
}

void GUITreeWalker::traverseComponent(
    const std::unique_ptr<GUIComponent> &component,
    std::function<void(const std::unique_ptr<GUIComponent> &component)>
        action) {
  action(component);

  const auto &components = component->getItems();

  for (const auto &component : components) {
    GUITreeWalker::traverseComponent(component, action);
  }
}

void GUITreeWalker::traverseComponent(
    const std::unique_ptr<GUIComponent> &parent,
    const std::unique_ptr<GUIComponent> &component,
    std::function<void(const std::unique_ptr<GUIComponent> &parent,
                       const std::unique_ptr<GUIComponent> &component)>
        action,
    bool &stop, bool &recurse) {
  if (stop) {
    return;
  }

  action(parent, component);

  if (recurse) {
    const auto &components = component->getItems();

    for (const auto &component : components) {
      GUITreeWalker::traverseComponent(parent, component, action, stop,
                                       recurse);
    }
  }
}

void GUITreeWalker::traverseComponent(
    const std::unique_ptr<GUIComponent> &component,
    std::function<void(const std::unique_ptr<GUIComponent> &parent,
                       const std::unique_ptr<GUIComponent> &component)>
        action,
    bool &stop, bool &recurse) {
  GUITreeWalker::traverseComponent(nullptr, component, action, stop, recurse);
}
