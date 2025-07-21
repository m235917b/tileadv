#include "gui/guielement.hpp"

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
