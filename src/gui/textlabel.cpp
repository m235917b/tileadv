#include "gui/textlabel.hpp"

TextLabel::TextLabel()
    : GUIElement(), text("")
{
}

TextLabel::TextLabel(const std::string& text)
    : GUIElement(), text(text)
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
