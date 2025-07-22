#include "gui/guiview.hpp"
#include "gui/guimenu.hpp"
#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"
#include "gui/asciiatlas.hpp"

GUIView::GUIView()
    : asciiGrey()
{
    
}

bool GUIView::init(SDL_Renderer* renderer)
{
    if(this->asciiGrey.loadFromFile("assets/ascii_grey.png", renderer) == false)
    {
        SDL_Log("Unable to load png image!\n");

        return false;
    }

    return true;
}

void GUIView::destroy()
{
    this->asciiGrey.destroy();
}

void GUIView::drawGUIMenu(SDL_Renderer* renderer, const GUIMenu& menu)
{
    for (const auto& container : menu.getMenuItems())
    {
        switch (container->getLayout())
        {
            case Layout::VERTICAL:
                drawVerticalLayout(renderer, container->getPosX(), container->getPosY(), *container.get());
                break;

            case Layout::HORIZONTAL:
                break;

            case Layout::GRID:
                break;

            default:
                break;
        }
    }
}

void GUIView::drawVerticalLayout(SDL_Renderer* renderer, const int posX, const int posY, const GUIContainer& container)
{
    int offset = 0;

    for (const std::unique_ptr<GUIElement>& element : container.getElements())
    {
        switch (element->getType())
        {
            case ElementType::TEXT:
                drawTextElement(renderer, posX, posY + offset, *element.get());
                break;

            default:
                break;
        }

        offset += element->getHeight();
    }
}

void GUIView::drawTextElement(SDL_Renderer* renderer, const int posX, const int posY, const GUIElement& element)
{
    drawText(renderer, posX, posY, 1.f, element.getText());
}

void GUIView::drawText(SDL_Renderer* renderer, const int posX, const int posY, const float size, const std::string& text)
{
    for (size_t i = 0; i < text.length(); ++i)
    {
        SDL_FRect spriteCoords = AsciiAtlas::getSpriteCoords(text[i]);
        float textPosX = posX + static_cast<float>(i) * asciiWidth * size;
        float textPosY = posY;

        asciiGrey.render(textPosX, textPosY, &spriteCoords, asciiWidth * size, asciiHeight * size, renderer);
    }
}
