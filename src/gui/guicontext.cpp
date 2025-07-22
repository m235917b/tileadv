#include "gui/guicontext.hpp"
#include "utils/rendercontext.hpp"
#include "gui/asciiatlas.hpp"

GUIContext::GUIContext(const RenderContext& renderContext)
    : guiView(renderContext), menus(), activeMenus()
{
    
}

const bool GUIContext::init()
{
    return guiView.init();
}

void GUIContext::keyDownListener(const SDL_Keycode key)
{
    for (const auto& [_, menu] : menus)
    {
        menu->keyDownListener(key);
    }
}

const bool GUIContext::addMenu(const std::string& id, std::unique_ptr<GUIMenu> menu)
{
    return menus.emplace(id, std::move(menu)).second;
}

const bool GUIContext::removeMenu(const std::string& id)
{
    return menus.erase(id) > 0;
}

const bool GUIContext::setMenuVisible(const std::string& id, const bool visible)
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

const bool GUIContext::drawGUI()
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
