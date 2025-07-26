#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "gui/guicomponent.hpp"

class GUITreeWalker {
public:
  static void traverse(GUIComponent &component,
                       std::function<bool(GUIComponent &component)> action);

  static void traverse(GUIComponent &component,
                       std::function<bool(GUIComponent &component)> action,
                       bool &stop);
};
