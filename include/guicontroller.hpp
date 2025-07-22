#pragma once

#include "gui/guicontext.hpp"
#include "gui/guimenu.hpp"

struct SDL_Renderer;

class GUIController
{
    public:
        GUIController();

        bool init(SDL_Renderer* renderer);
        void destroy();

        void initGameMenus();

        void keyDownListener(const SDL_Keycode& key);

        void setMainMenuVisible(const bool visible);
        void setCharacterMenuVisible(const bool visible);

        void drawMenus(SDL_Renderer* renderer);

    private:
        GUIContext guiContext;
};
