#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "gui/guicomponent.hpp"

class GUITreeTraverser {
    public:
        static void traverse(const std::vector<std::unique_ptr<GUIComponent>>& components, std::function<void(const std::unique_ptr<GUIComponent>& component)> action);
        static void traverseComponent(const std::unique_ptr<GUIComponent>& component, std::function<void(const std::unique_ptr<GUIComponent>& component)> action);
};
