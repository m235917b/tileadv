#pragma once

#include "staterunner.hpp"
#include "gui/guicontext.hpp"
#include "gui/guimenu.hpp"

class RenderContext;
struct SDL_Renderer;

class GUIController : public StateRunner
{
    public:
        GUIController(RenderContext* renderContext);

        bool init();
        void destroy();

        void initGameMenus();

        void keyDownListener(const SDL_Keycode& key);

        void setMainMenuVisible(const bool visible);
        void setCharacterMenuVisible(const bool visible);

        int run() override;

    private:
        GUIContext guiContext;
};
