#include "gui/guimenu.hpp"

GUIMenu::GUIMenu(const std::string& id)
    : id(id)
{

}

const bool GUIMenu::addMenuItem(std::unique_ptr<GUIContainer> container)
{
    return menuItems.emplace(container->getId(), std::move(container)).second;
}

const bool GUIMenu::removeMenuItem(const std::string& id)
{
    return menuItems.erase(id) > 0;
}

void GUIMenu::update()
{
    for (const auto& [_, item] : menuItems)
    {
        item->update();
    }
}

void GUIMenu::keyDownListener(const SDL_Keycode key)
{
    for (const auto& [_, item] : menuItems)
    {
        item->keyDownListener(key);
    }
}

const std::unordered_map<std::string, std::unique_ptr<GUIContainer>>& GUIMenu::getMenuItems() const
{
    return menuItems;
}
