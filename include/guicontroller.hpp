#pragma once

#include "staterunner.hpp"
#include "gui/guicontext.hpp"

class RenderContext;
struct SDL_Renderer;

class GUIController : public StateRunner
{
    public:
        GUIController(const RenderContext& renderContext);

        const bool init();

        void initGameMenus();

        void keyDownListener(const SDL_Keycode key);

        void setMainMenuVisible(const bool visible);
        void setCharacterMenuVisible(const bool visible);

        const int run() override;

    private:
        GUIContext guiContext;
};
