#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <set>

#include <SDL3/SDL.h>

#include "gui/guiview.hpp"
#include "gui/guimenu.hpp"

class RenderContext;

class GUIContext
{
    public:
        GUIContext(const RenderContext& renderContext);

        const bool init();

        void keyDownListener(const SDL_Keycode key);

        const bool addMenu(const std::string& id, std::unique_ptr<GUIMenu> menu);
        const bool removeMenu(const std::string& id);

        const bool setMenuVisible(const std::string& id, const bool visible);

        const bool drawGUI();

    private:
        GUIView guiView;

        std::unordered_map<std::string, std::unique_ptr<GUIMenu>> menus;
        std::set<std::string> activeMenus;
};