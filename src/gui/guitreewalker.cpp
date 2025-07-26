#include "gui/guitreewalker.hpp"

void GUITreeWalker::traverse(
    GUIComponent &component,
    std::function<bool(GUIComponent &component)> action) {
  const auto recurse{action(component)};

  if (recurse) {
    component.forEachChild([&action](GUIComponent &child) {
      GUITreeWalker::traverse(child, action);
    });
  }
}

void GUITreeWalker::traverse(
    GUIComponent &component,
    std::function<bool(GUIComponent &component)> action, bool &stop) {
  if (stop) {
    return;
  }

  const auto recurse{action(component)};

  if (recurse) {
    component.forEachChild([&action, &stop](GUIComponent &child) {
      GUITreeWalker::traverse(child, action, stop);
    });
  }
}
