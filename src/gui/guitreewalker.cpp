#include "gui/guitreewalker.hpp"

void GUITreeWalker::traverse(
    GUIComponent &component,
    std::function<void(GUIComponent &component)> action) {
  action(component);

  component.forEachChild([action](GUIComponent &child) {
    GUITreeWalker::traverse(child, action);
  });
}

void GUITreeWalker::traverse(
    GUIComponent &component,
    std::function<void(GUIComponent &component)> action,
    std::function<bool(const GUIComponent &component)> recurse, bool &stop) {
  if (stop) {
    return;
  }

  action(component);

  if (recurse(component)) {
    component.forEachChild([action, recurse, &stop](GUIComponent &child) {
      GUITreeWalker::traverse(child, action, recurse, stop);
    });
  }
}
