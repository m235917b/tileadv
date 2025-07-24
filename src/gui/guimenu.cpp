#include "gui/guimenu.hpp"

GUIMenu::GUIMenu(const std::string& id)
    : id(id), menuItems(), selectedContainer(menuItems.begin())
{

}

bool GUIMenu::addMenuItem(std::unique_ptr<GUIContainer> container)
{
    if (!menuItems.empty())
    {
        selectedContainer->second->setSelected(false);
    }

    auto retVal{ menuItems.emplace(container->getId(), std::move(container)).second };
    selectedContainer = menuItems.begin();
    selectedContainer->second->setSelected(true);

    return retVal;
}

bool GUIMenu::removeMenuItem(const std::string& id)
{
    if (!menuItems.empty())
    {
        selectedContainer->second->setSelected(false);
    }

    auto retVal{ menuItems.erase(id) > 0 };
    selectedContainer = menuItems.begin();
    selectedContainer->second->setSelected(true);

    return retVal;
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
    if (!menuItems.empty() && selectedContainer->second->isActive())
    {
        switch (key)
        {
            case SDLK_BACKSPACE:
                selectedContainer->second->setActive(false);
                break;

            default:
                selectedContainer->second->keyDownListener(key);
                break;
        }
    }
    else if (!menuItems.empty())
    {
        switch (key)
        {
            case SDLK_RIGHT:
                selectRight();
                break;

            case SDLK_LEFT:
                selectLeft();

                break;

            case SDLK_RETURN:
                selectedContainer->second->setActive(true);
                break;
        }
    }
}

const std::map<std::string, std::unique_ptr<GUIContainer>>& GUIMenu::getMenuItems() const
{
    return menuItems;
}

void GUIMenu::selectRight()
{
    if (menuItems.empty())
    {
        return;
    }

    selectedContainer->second->setSelected(false);

    selectedContainer++;

    if (selectedContainer == menuItems.end())
    {
        selectedContainer = menuItems.begin();
    }

    selectedContainer->second->setSelected(true);
}

void GUIMenu::selectLeft()
{
    if (menuItems.empty())
    {
        return;
    }

    selectedContainer->second->setSelected(false);

    if (selectedContainer == menuItems.begin())
    {
        selectedContainer = menuItems.end();
    }

    selectedContainer--;

    selectedContainer->second->setSelected(true);
}
