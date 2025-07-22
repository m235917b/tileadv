#include "gui/textlabel.hpp"

TextLabel::TextLabel(int width, int height)
    : GUIElement(width, height), text("")
{
    
}

TextLabel::TextLabel(int width, int height, const std::string& text)
    : GUIElement(width, height), text(text)
{

}

void TextLabel::setText(const std::string& text)
{
    this->text = text;
}

const std::string& TextLabel::getText() const
{
    return text;
}
