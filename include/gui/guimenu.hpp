#pragma once

#include <string>
#include <map>
#include <memory>

#include "gui/guicontainer.hpp"

class GUIMenu
{
    public:
        GUIMenu(const std::string& id);

        bool addMenuItem(std::unique_ptr<GUIContainer> container);
        bool removeMenuItem(const std::string& id);
        void update();
        void keyDownListener(const SDL_Keycode key);

        const std::map<std::string, std::unique_ptr<GUIContainer>>& getMenuItems() const;

    private:
        const std::string id;

        std::map<std::string, std::unique_ptr<GUIContainer>> menuItems;

        std::map<std::string, std::unique_ptr<GUIContainer>>::iterator selectedContainer;

        void selectRight();
        void selectLeft();
};