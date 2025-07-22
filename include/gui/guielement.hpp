#pragma once

#include <functional>
#include <unordered_map>
#include <string>

#include <SDL3/SDL.h>

class View;

enum class ElementType
{
    TEXT
};

class GUIElement
{
    public:
        GUIElement(ElementType type, int width, int height);
        GUIElement(ElementType type, int width, int height, std::string text);

        void setUpdateListener(std::function<void()> listener);
        void addKeyListener(const SDL_Keycode& key, std::function<void()> listener);
        void update();
        void keyDownListener(const SDL_Keycode& key);

        void setText(const std::string& text);
        const std::string& getText() const;
        const ElementType getType() const;
        const int getWidth() const;
        const int getHeight() const;

    private:
        ElementType type;

        int width;
        int height;

        std::function<void()> updateListener;
        std::unordered_map<SDL_Keycode, std::function<void()>> keyListeners;

        std::string text;
};
