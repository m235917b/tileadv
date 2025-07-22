#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "gui/guicontainer.hpp"

class GUIMenu
{
    public:
        GUIMenu(const std::string& id);

        const bool addMenuItem(std::unique_ptr<GUIContainer> container);
        const bool removeMenuItem(const std::string& id);
        void update();
        void keyDownListener(const SDL_Keycode key);

        const std::unordered_map<std::string, std::unique_ptr<GUIContainer>>& getMenuItems() const;

    private:
        const std::string id;

        std::unordered_map<std::string, std::unique_ptr<GUIContainer>> menuItems;
};