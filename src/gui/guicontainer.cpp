#include <memory>

#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"

GUIContainer::GUIContainer(const std::string& id, const Layout layout, const int posX, const int posY, const int width, const int height) :
    id(id),
    layout(layout),
    posX(posX),
    posY(posY),
    width(width),
    height(height),
    border(false),
    background(false),
    selected(false),
    active(false),
    elements(),
    selectedElement(elements.begin())
{

}

bool GUIContainer::addElement(std::unique_ptr<GUIElement> element)
{
    if (!elements.empty())
    {
        selectedElement->second->setSelected(false);
    }

    auto retVal{ elements.emplace(element->getId(), std::move(element)).second };
    selectedElement = elements.begin();

    return retVal;
}

bool GUIContainer::removeElement(const std::string& id)
{
    if (!elements.empty())
    {
        selectedElement->second->setSelected(false);
    }

    auto retVal{ elements.erase(id) > 0 };
    selectedElement = elements.begin();

    return retVal;
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
    switch (key)
    {
        case SDLK_DOWN:
            if (!elements.empty())
            {
                selectRight();
            }
            break;

        case SDLK_UP:
            if (!elements.empty())
            {
                selectLeft();
            }
            break;

        default:
            if (!elements.empty())
            {
                selectedElement->second->keyDownListener(key);
            }
            break;
    }
}

void GUIContainer::setBorder(const bool visible)
{
    border = visible;
}

void GUIContainer::setBackground(const bool visible)
{
    background = visible;
}

void GUIContainer::setSelected(const bool selected)
{
    this->selected = selected;
}

void GUIContainer::setActive(const bool active)
{
    this->active = active;
}

std::string GUIContainer::getId() const
{
    return id;
}

Layout GUIContainer::getLayout() const {
    return layout;
}

const std::map<std::string, std::unique_ptr<GUIElement>>& GUIContainer::getElements() const
{
    return elements;
}

int GUIContainer::getPosX() const
{
    return posX;
}

int GUIContainer::getPosY() const
{
    return posY;
}

int GUIContainer::getWidth() const
{
    return width;
}

int GUIContainer::getHeight() const
{
    return height;
}

bool GUIContainer::getBorder() const
{
    return border;
}

bool GUIContainer::getBackground() const
{
    return background;
}

bool GUIContainer::isSelected() const
{
    return selected;
}

bool GUIContainer::isActive() const
{
    return active;
}

void GUIContainer::selectRight()
{
    if (elements.empty())
    {
        return;
    }

    selectedElement->second->setSelected(false);

    selectedElement++;

    if (selectedElement == elements.end())
    {
        selectedElement = elements.begin();
    }

    selectedElement->second->setSelected(true);
}

void GUIContainer::selectLeft()
{
    if (elements.empty())
    {
        return;
    }

    selectedElement->second->setSelected(false);

    if (selectedElement == elements.begin())
    {
        selectedElement = elements.end();
    }

    selectedElement--;

    selectedElement->second->setSelected(true);
}
