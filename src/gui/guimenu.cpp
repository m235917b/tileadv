#include <algorithm>

#include "gui/guimenu.hpp"

void GUIMenu::addMenuItem(std::unique_ptr<GUIContainer> container)
{
    menuItems.push_back(std::move(container));
}

void GUIMenu::removeMenuItem(GUIContainer* container)
{
    menuItems.erase(std::remove_if(menuItems.begin(), menuItems.end(),
                                   [container](const std::unique_ptr<GUIContainer>& item) { return item.get() == container; }),
                    menuItems.end());
}

void GUIMenu::update()
{
    for (const auto& item : menuItems)
    {
        item->update();
    }
}

void GUIMenu::keyDownListener(const SDL_Keycode& key)
{
    for (const auto& item : menuItems)
    {
        item->keyDownListener(key);
    }
}

const std::vector<std::unique_ptr<GUIContainer>>& GUIMenu::getMenuItems() const
{
    return menuItems;
}
