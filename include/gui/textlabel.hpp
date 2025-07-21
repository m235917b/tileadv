#pragma once

#include <string>
#include <functional>

#include <SDL3/SDL.h>

#include "gui/guielement.hpp"

class TextLabel : public GUIElement
{
    public:
        TextLabel();
        TextLabel(const std::string& text);

        void setText(const std::string& text);
        const std::string& getText() const;

    private:
        std::string text;
};