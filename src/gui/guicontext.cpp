#include <SDL3/SDL.h>

#include "gui/guicontext.hpp"
#include "utils/rendercontext.hpp"
#include "gui/asciiatlas.hpp"

GUIContext::GUIContext(const RenderContext& renderContext)
    : guiView(renderContext), menus(), activeMenus()
{
    
}

bool GUIContext::init()
{
    return this->guiView.init();
}

void GUIContext::keyDownListener(const SDL_Keycode& key)
{

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
        if (menus.find(id) == menus.end())
        {
            return false;
        }

        menus[id]->update();
        guiView.drawGUIMenu(*menus[id].get());
    }

    return true;
}
