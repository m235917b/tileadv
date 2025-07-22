#include <memory>

#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"

GUIContainer::GUIContainer(const std::string& id, const Layout layout, const int posX, const int posY, const int width, const int height)
    : id(id), layout(layout), posX(posX), posY(posY), width(width), height(height)
{

}

bool GUIContainer::addElement(std::unique_ptr<GUIElement> element)
{
    return elements.emplace(element->getId(), std::move(element)).second;
}

bool GUIContainer::removeElement(const std::string& id)
{
    return elements.erase(id) > 0;
}

void GUIContainer::update()
{
    for (const auto& [_, element] : elements)
    {
        element->update();
    }
}

void GUIContainer::keyDownListener(const SDL_Keycode key)
{
    for (const auto& [_, element] : elements)
    {
        element->keyDownListener(key);
    }
}

const std::string GUIContainer::getId() const
{
    return id;
}

const Layout GUIContainer::getLayout() const {
    return layout;
}

const std::unordered_map<std::string, std::unique_ptr<GUIElement>>& GUIContainer::getElements() const
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
