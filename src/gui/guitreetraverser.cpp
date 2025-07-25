#include "gui/guitreetraverser.hpp"

void GUITreeTraverser::traverse(const std::vector<std::unique_ptr<GUIComponent>>& components, std::function<void(const std::unique_ptr<GUIComponent>& component)> action) {
    for (const auto& component : components) {
        GUITreeTraverser::traverseComponent(component, action);
    }
}

void GUITreeTraverser::traverseComponent(const std::unique_ptr<GUIComponent>& component, std::function<void(const std::unique_ptr<GUIComponent>& component)> action) {
    action(component);

    const auto& components = component->getItems();

    for (const auto& component : components) {
        GUITreeTraverser::traverseComponent(component, action);
    }
}
