#include <memory>
#include <algorithm>

#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"

GUIContainer::GUIContainer(Layout layout, int posX, int posY, int width, int height)
    : layout(layout), posX(posX), posY(posY), width(width), height(height)
{

}

void GUIContainer::addElement(std::unique_ptr<GUIElement> element)
{
    elements.push_back(std::move(element));
}

void GUIContainer::removeElement(GUIElement* element)
{
    elements.erase(std::remove_if(elements.begin(), elements.end(),
                                   [element](const std::unique_ptr<GUIElement>& e) { return e.get() == element; }),
                   elements.end());
}

void GUIContainer::update()
{
    for (const auto& element : elements)
    {
        element->update();
    }
}

void GUIContainer::keyDownListener(const SDL_Keycode& key)
{
    for (const auto& element : elements)
    {
        element->keyDownListener(key);
    }
}

const Layout GUIContainer::getLayout() const {
    return layout;
}

const std::vector<std::unique_ptr<GUIElement>>& GUIContainer::getElements() const
{
    return elements;
}

const int GUIContainer::getPosX() const
{
    return posX;
}

const int GUIContainer::getPosY() const
{
    return posY;
}

const int GUIContainer::getWidth() const
{
    return width;
}

const int GUIContainer::getHeight() const
{
    return height;
}
