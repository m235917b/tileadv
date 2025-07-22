#pragma once

#include "utils/rendercontext.hpp"
#include "utils/ltexture.hpp"

struct SDL_Renderer;

class GUIMenu;
class GUIContainer;
class GUIElement;

class GUIView
{
    public:
        GUIView(RenderContext* renderContext);

        bool init();
        void destroy();

        void drawGUIMenu(const GUIMenu& menu);
        void drawVerticalLayout(const int posX, const int posY, const GUIContainer& container);
        void drawTextElement(const int posX, const int posY, const GUIElement& element);
        void drawText(const int posX, const int posY, const float size, const std::string& text);

    private:
        RenderContext* renderContext;

        LTexture asciiGrey;
};
