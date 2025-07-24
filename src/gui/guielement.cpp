#include "gui/guielement.hpp"

GUIElement::GUIElement(const std::string& id, const ElementType type, const int width, const int height) :
    id(id),
    type(type),
    width(width),
    height(height),
    selected(false),
    text("")
{
    
}

GUIElement::GUIElement(const std::string& id, const ElementType type, const int width, const int height, const std::string& text) :
    id(id),
    type(type),
    width(width),
    height(height),
    selected(false),
    text(text)
{
    
}

void GUIElement::setUpdateListener(std::function<void()> listener)
{
    updateListener = std::move(listener);
}

void GUIElement::addKeyListener(const SDL_Keycode key, std::function<void()> listener)
{
    keyListeners[key] = std::move(listener);
}

void GUIElement::update()
{
    if (updateListener)
    {
        updateListener();
    }
}

void GUIElement::keyDownListener(const SDL_Keycode key)
{
    auto it = keyListeners.find(key);
    if (it != keyListeners.end())
    {
        it->second();
    }
}

void GUIElement::setText(const std::string& text)
{
    this->text = text;
}

void GUIElement::setSelected(const bool selected)
{
    this->selected = selected;
}

const std::string& GUIElement::getId() const
{
    return id;
}

const std::string& GUIElement::getText() const
{
    return text;
}

ElementType GUIElement::getType() const
{
    return type;
}

int GUIElement::getWidth() const
{
    return width;
}

int GUIElement::getHeight() const
{
    return height;
}

bool GUIElement::isSelected() const
{
    return selected;
}
