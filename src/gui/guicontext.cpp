#include "gui/guicontext.hpp"
#include "utils/rendercontext.hpp"
#include "gui/asciiatlas.hpp"

GUIContext::GUIContext(const RenderContext& renderContext)
    : guiView(renderContext), menus(), activeMenus()
{
    
}

bool GUIContext::init()
{
    return guiView.init();
}

void GUIContext::keyDownListener(const SDL_Keycode key)
{
    for (const auto& id : activeMenus)
    {
        const auto& menu = menus.find(id);

        if (menu == menus.end())
        {
            return;
        }

        menu->second->keyDownListener(key);
    }
}

void GUIContext::addKeyListener(const std::string& id, const SDL_Keycode key, std::function<void()> listener)
{
    for (const auto& menu : menus)
    {
        for (const auto& [_, container] : menu.second->getMenuItems())
        {
            for (const auto& [_, element] : container->getElements())
            {
                if (element->getId() == id)
                {
                    element->addKeyListener(key, listener);
                }
            }
        }
    }
}

bool GUIContext::addMenu(const std::string& id, std::unique_ptr<GUIMenu> menu)
{
    return menus.emplace(id, std::move(menu)).second;
}

bool GUIContext::removeMenu(const std::string& id)
{
    return menus.erase(id) > 0;
}

bool GUIContext::setMenuVisible(const std::string& id, const bool visible)
{
    if (visible)
    {
        return activeMenus.emplace(id).second;
    }
    else
    {
        return activeMenus.erase(id) > 0;
    }
}

bool GUIContext::drawGUI()
{
    for (const auto& id : activeMenus)
    {
        const auto& menu = menus.find(id);
        
        if (menu == menus.end())
        {
            return false;
        }

        menu->second->update();
        guiView.drawGUIMenu(*menu->second);
    }

    return true;
}
