#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <set>

#include <SDL3/SDL.h>

#include "gui/guiview.hpp"
#include "gui/guimenu.hpp"

class GUIContext
{
    public:
        GUIContext();

        bool init(SDL_Renderer* renderer);
        void destroy();

        void keyDownListener(const SDL_Keycode& key);

        bool addMenu(const std::string& id, std::unique_ptr<GUIMenu> menu);
        bool removeMenu(const std::string& id);

        bool setMenuVisible(const std::string& id, const bool visible);

        bool drawGUI(SDL_Renderer* renderer);

    private:
        GUIView guiView;

        std::unordered_map<std::string, std::unique_ptr<GUIMenu>> menus;
        std::set<std::string> activeMenus;
};