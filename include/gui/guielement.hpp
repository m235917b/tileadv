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
        GUIElement(const std::string& id, const ElementType type, const int width, const int height);
        GUIElement(const std::string& id, const ElementType type, const int width, const int height, const std::string& text);

        void setUpdateListener(std::function<void()> listener);
        void addKeyListener(const SDL_Keycode key, std::function<void()> listener);
        void update();
        void keyDownListener(const SDL_Keycode key);

        void setText(const std::string& text);
        void setSelected(const bool selected);

        const std::string& getId() const;
        const std::string& getText() const;
        ElementType getType() const;
        int getWidth() const;
        int getHeight() const;
        bool isSelected() const;

    private:
        const std::string id;

        ElementType type;

        int width;
        int height;

        bool selected;

        std::function<void()> updateListener;
        std::unordered_map<SDL_Keycode, std::function<void()>> keyListeners;

        std::string text;
};
