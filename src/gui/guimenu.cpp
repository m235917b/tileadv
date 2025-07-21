#include <algorithm>

#include "gui/guimenu.hpp"

void GUIMenu::addMenuItem(int x, int y, std::unique_ptr<GUIContainer> container)
{
    menuItems.emplace_back(x, y, std::move(container));
}

void GUIMenu::removeMenuItem(GUIContainer* container)
{
    menuItems.erase(std::remove_if(menuItems.begin(), menuItems.end(),
                                    [container](const auto& item) { return std::get<2>(item).get() == container; }),
                    menuItems.end());
}

void GUIMenu::update()
{
    for (const auto& item : menuItems)
    {
        std::get<2>(item)->update();
    }
}

void GUIMenu::keyDownListener(const SDL_Keycode& key)
{
    for (const auto& item : menuItems)
    {
        std::get<2>(item)->keyDownListener(key);
    }
}
