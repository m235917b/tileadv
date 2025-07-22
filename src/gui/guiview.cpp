#include <SDL3/SDL.h>

#include "gui/guiview.hpp"
#include "utils/rendercontext.hpp"
#include "gui/guimenu.hpp"
#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"
#include "gui/asciiatlas.hpp"

GUIView::GUIView(RenderContext* renderContext)
    : renderContext(renderContext), asciiGrey()
{
    
}

bool GUIView::init()
{
    SDL_Renderer* renderer{ this->renderContext->getRenderer() };

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

void GUIView::drawGUIMenu(const GUIMenu& menu)
{
    for (const auto& container : menu.getMenuItems())
    {
        switch (container->getLayout())
        {
            case Layout::VERTICAL:
                drawVerticalLayout(container->getPosX(), container->getPosY(), *container.get());
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

void GUIView::drawVerticalLayout(const int posX, const int posY, const GUIContainer& container)
{
    int offset = 0;

    for (const std::unique_ptr<GUIElement>& element : container.getElements())
    {
        switch (element->getType())
        {
            case ElementType::TEXT:
                drawTextElement(posX, posY + offset, *element.get());
                break;

            default:
                break;
        }

        offset += element->getHeight();
    }
}

void GUIView::drawTextElement(const int posX, const int posY, const GUIElement& element)
{
    drawText(posX, posY, 1.f, element.getText());
}

void GUIView::drawText(const int posX, const int posY, const float size, const std::string& text)
{
    SDL_Renderer* renderer{ this->renderContext->getRenderer() };
    
    for (size_t i = 0; i < text.length(); ++i)
    {
        SDL_FRect spriteCoords = AsciiAtlas::getSpriteCoords(text[i]);
        float textPosX = posX + static_cast<float>(i) * asciiWidth * size;
        float textPosY = posY;

        asciiGrey.render(textPosX, textPosY, &spriteCoords, asciiWidth * size, asciiHeight * size, renderer);
    }
}
