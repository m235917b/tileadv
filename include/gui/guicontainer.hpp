#pragma once

#include <SDL3/SDL.h>

class GUIContainer
{
    public:
        GUIContainer() = default;

        virtual void update() = 0;
        virtual void keyDownListener(const SDL_Keycode& key) = 0;
};