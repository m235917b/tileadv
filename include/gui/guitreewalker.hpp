#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "gui/guicomponent.hpp"

class GUITreeWalker {
public:
  static void
  traverse(const std::vector<std::unique_ptr<GUIComponent>> &components,
           std::function<void(const std::unique_ptr<GUIComponent> &component)>
               action);

  static void
  traverse(const std::vector<std::unique_ptr<GUIComponent>> &components,
           std::function<void(const std::unique_ptr<GUIComponent> &parent,
                              const std::unique_ptr<GUIComponent> &component)>
               action,
           std::function<bool(const std::unique_ptr<GUIComponent> &component)>
               recurse,
           bool &stop);

  static void traverseComponent(
      const std::unique_ptr<GUIComponent> &component,
      std::function<void(const std::unique_ptr<GUIComponent> &component)>
          action);

  static void traverseComponent(
      const std::unique_ptr<GUIComponent> &component,
      std::function<void(const std::unique_ptr<GUIComponent> &parent,
                         const std::unique_ptr<GUIComponent> &component)>
          action,
      std::function<bool(const std::unique_ptr<GUIComponent> &component)>
          recurse,
      bool &stop);

private:
  static void traverseComponent(
      const std::unique_ptr<GUIComponent> &parent,
      const std::unique_ptr<GUIComponent> &component,
      std::function<void(const std::unique_ptr<GUIComponent> &parent,
                         const std::unique_ptr<GUIComponent> &component)>
          action,
      std::function<bool(const std::unique_ptr<GUIComponent> &component)>
          recurse,
      bool &stop);
};
