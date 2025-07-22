#include "gui/guicontext.hpp"
#include "gui/asciiatlas.hpp"

GUIContext::GUIContext()
    : guiView(), menus(), activeMenus()
{
    
}

bool GUIContext::init(SDL_Renderer* renderer)
{
    return this->guiView.init(renderer);
}

void GUIContext::destroy()
{
    this->guiView.destroy();
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

bool GUIContext::drawGUI(SDL_Renderer* renderer)
{
    for (const auto& id : activeMenus)
    {
        if (menus.find(id) == menus.end())
        {
            return false;
        }

        menus[id]->update();
        guiView.drawGUIMenu(renderer, *menus[id].get());
    }

    return true;
}
