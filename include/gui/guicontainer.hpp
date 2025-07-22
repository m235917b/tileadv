#pragma once

#include <SDL3/SDL.h>

class GUIContainer
{
    public:
        GUIContainer(int posX, int posY, int width, int height);

        virtual void update() = 0;
        virtual void keyDownListener(const SDL_Keycode& key) = 0;

    private:
        int posX;
        int posY;
        int width;
        int height;
};