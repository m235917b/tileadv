#pragma once

#include <vector>
#include <tuple>
#include <memory>

#include "gui/guicontainer.hpp"

class GUIMenu
{
    public:
        GUIMenu() = default;

        void addMenuItem(std::unique_ptr<GUIContainer> container);
        void removeMenuItem(GUIContainer* container);
        void update();
        void keyDownListener(const SDL_Keycode& key);

        const std::vector<std::unique_ptr<GUIContainer>>& getMenuItems() const;

    private:
        std::vector<std::unique_ptr<GUIContainer>> menuItems;
};