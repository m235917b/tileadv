#pragma once

#include <string>
#include <map>

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

        void setBorder(const bool visible);
        void setBackground(const bool visible);
        void setSelected(const bool selected);
        void setActive(const bool active);

        std::string getId() const;
        Layout getLayout() const;
        const std::map<std::string, std::unique_ptr<GUIElement>>& getElements() const;
        int getPosX() const;
        int getPosY() const;
        int getWidth() const;
        int getHeight() const;
        bool getBorder() const;
        bool getBackground() const;
        bool isSelected() const;
        bool isActive() const;

    private:
        const std::string id;

        Layout layout;

        int posX;
        int posY;
        int width;
        int height;

        bool border;
        bool background;

        bool selected;
        bool active;

        std::map<std::string, std::unique_ptr<GUIElement>> elements;

        std::map<std::string, std::unique_ptr<GUIElement>>::iterator selectedElement;

        void selectRight();
        void selectLeft();
};