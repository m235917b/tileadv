#pragma once

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
        GUIContainer(Layout layout, int posX, int posY, int width, int height);

        void addElement(std::unique_ptr<GUIElement> element);
        void removeElement(GUIElement* element);
        void update();
        void keyDownListener(const SDL_Keycode& key);

        const Layout getLayout() const;
        const std::vector<std::unique_ptr<GUIElement>>& getElements() const;
        const int getPosX() const;
        const int getPosY() const;
        const int getWidth() const;
        const int getHeight() const;

    private:
        Layout layout;

        int posX;
        int posY;
        int width;
        int height;

        bool border;

        std::vector<std::unique_ptr<GUIElement>> elements;
};