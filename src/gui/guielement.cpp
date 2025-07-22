#include "gui/guielement.hpp"

GUIElement::GUIElement(ElementType type, int width, int height)
    : type(type), width(width), height(height), text("")
{
    
}

GUIElement::GUIElement(ElementType type, int width, int height, std::string text)
    : type(type), width(width), height(height), text(text)
{
    
}

void GUIElement::setUpdateListener(std::function<void()> listener)
{
    updateListener = listener;
}

void GUIElement::addKeyListener(const SDL_Keycode& key, std::function<void()> listener)
{
    keyListeners[key] = listener;
}

void GUIElement::update()
{
    if (updateListener)
    {
        updateListener();
    }
}

void GUIElement::keyDownListener(const SDL_Keycode& key)
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

const std::string& GUIElement::getText() const
{
    return text;
}

const ElementType GUIElement::getType() const
{
    return type;
}

const int GUIElement::getWidth() const
{
    return width;
}

const int GUIElement::getHeight() const
{
    return height;
}
