#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <set>
#include <functional>

#include <SDL3/SDL.h>

#include "gui/guiview.hpp"
#include "gui/guimenu.hpp"

class RenderContext;

class GUIContext
{
    public:
        GUIContext(const RenderContext& renderContext);

        bool init();

        void keyDownListener(const SDL_Keycode key);
        void addKeyListener(const std::string& id, const SDL_Keycode key, std::function<void()> listener);

        bool addMenu(const std::string& id, std::unique_ptr<GUIMenu> menu);
        bool removeMenu(const std::string& id);

        bool setMenuVisible(const std::string& id, const bool visible);

        bool drawGUI();

    private:
        GUIView guiView;

        std::unordered_map<std::string, std::unique_ptr<GUIMenu>> menus;
        std::set<std::string> activeMenus;
};