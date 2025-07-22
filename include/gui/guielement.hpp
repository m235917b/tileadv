#pragma once

#include <functional>
#include <unordered_map>

#include <SDL3/SDL.h>

class GUIElement
{
    public:
        GUIElement(int width, int height);

        void setUpdateListener(std::function<void()> listener);
        void addKeyListener(const SDL_Keycode& key, std::function<void()> listener);
        void update();
        void keyDownListener(const SDL_Keycode& key);

    private:
        int width;
        int height;

        std::function<void()> updateListener;
        std::unordered_map<SDL_Keycode, std::function<void()>> keyListeners;
};
