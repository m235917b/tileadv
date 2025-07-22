#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

#include "gui/guielement.hpp"

enum class Layout
{
    VERTICAL,
    HORIZONTAL,
    GRID
};

class GUIContainer
{
    public:
        GUIContainer(const std::string& id, const Layout layout, const int posX, const int posY, const int width, const int height);

        bool addElement(std::unique_ptr<GUIElement> element);
        bool removeElement(const std::string& id);
        void update();
        void keyDownListener(const SDL_Keycode key);

        const std::string getId() const;
        const Layout getLayout() const;
        const std::unordered_map<std::string, std::unique_ptr<GUIElement>>& getElements() const;
        const int getPosX() const;
        const int getPosY() const;
        const int getWidth() const;
        const int getHeight() const;

    private:
        const std::string id;

        Layout layout;

        int posX;
        int posY;
        int width;
        int height;

        bool border;

        std::unordered_map<std::string, std::unique_ptr<GUIElement>> elements;
};