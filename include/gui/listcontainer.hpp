#pragma once

#include <vector>
#include <memory>

#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"

class ListContainer : public GUIContainer
{
    public:
        ListContainer(int posX, int posY, int width, int height);

        void addElement(std::unique_ptr<GUIElement> element);
        void removeElement(GUIElement* element);
        void update() override;
        void keyDownListener(const SDL_Keycode& key) override;

    private:
        std::vector<std::unique_ptr<GUIElement>> elements;
};
