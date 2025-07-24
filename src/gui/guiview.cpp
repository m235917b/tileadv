#include "gui/guiview.hpp"
#include "utils/rendercontext.hpp"
#include "gui/guimenu.hpp"
#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"
#include "gui/asciiatlas.hpp"

GUIView::GUIView(const RenderContext& renderContext)
    : renderContext(renderContext), asciiGrey()
{
    
}

bool GUIView::init()
{
    SDL_Renderer& renderer{ renderContext.getRenderer() };

    if(asciiGrey.loadFromFile("guiassets/ascii_grey.png", &renderer) == false)
    {
        SDL_Log("Unable to load png image!\n");

        return false;
    }

    return true;
}

void GUIView::drawGUIMenu(const GUIMenu& menu)
{
    for (const auto& [_, container] : menu.getMenuItems())
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
    SDL_Renderer& renderer{ renderContext.getRenderer() };

    if (container.getBorder())
    {
        SDL_FRect rect;

        rect.x = container.getPosX();
        rect.y = container.getPosY();
        rect.w = container.getWidth();
        rect.h = container.getHeight();

        SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
        SDL_RenderRect(&renderer, &rect);
    }

    if (container.getBackground())
    {
        SDL_FRect rect;

        rect.x = container.getPosX();
        rect.y = container.getPosY();
        rect.w = container.getWidth();
        rect.h = container.getHeight();

        SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
        SDL_RenderFillRect(&renderer, &rect);
    }

    if (container.isSelected())
    {
        SDL_FRect rect;

        rect.x = container.getPosX();
        rect.y = container.getPosY();
        rect.w = container.getWidth();
        rect.h = container.getHeight();

        SDL_SetRenderDrawColor(&renderer, 0x50, 0x50, 0x50, 0xFF);
        SDL_RenderRect(&renderer, &rect);
    }

    if (container.isActive())
    {
        SDL_FRect rect;

        rect.x = container.getPosX();
        rect.y = container.getPosY();
        rect.w = container.getWidth();
        rect.h = container.getHeight();

        SDL_SetRenderDrawColor(&renderer, 0xFF, 0x40, 0x40, 0xFF);
        SDL_RenderRect(&renderer, &rect);
    }

    int offset = 0;

    for (const auto& [_, element] : container.getElements())
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
    SDL_Renderer& renderer{ renderContext.getRenderer() };

    const auto text{ (element.isSelected() ? " > " : "   ") + element.getText() };

    drawText(posX, posY, 1.f, text);
}

void GUIView::drawText(const int posX, const int posY, const float size, const std::string& text)
{
    SDL_Renderer& renderer{ renderContext.getRenderer() };

    for (size_t i = 0; i < text.length(); ++i)
    {
        SDL_FRect spriteCoords = AsciiAtlas::getSpriteCoords(text[i]);
        float textPosX = posX + static_cast<float>(i) * asciiWidth * size;
        float textPosY = posY;

        asciiGrey.render(textPosX, textPosY, &spriteCoords, asciiWidth * size, asciiHeight * size, &renderer);
    }
}
