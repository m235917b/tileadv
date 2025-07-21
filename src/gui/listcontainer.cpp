#include <algorithm>

#include "gui/listcontainer.hpp"

void ListContainer::addElement(std::unique_ptr<GUIElement> element)
{
    elements.push_back(std::move(element));
}

void ListContainer::removeElement(GUIElement* element)
{
    elements.erase(std::remove_if(elements.begin(), elements.end(),
                                   [element](const std::unique_ptr<GUIElement>& e) { return e.get() == element; }),
                   elements.end());
}

void ListContainer::update()
{
    for (const auto& element : elements)
    {
        element->update();
    }
}

void ListContainer::keyDownListener(const SDL_Keycode& key)
{
    for (const auto& element : elements)
    {
        element->keyDownListener(key);
    }
}
