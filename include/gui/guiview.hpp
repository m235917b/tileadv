#pragma once

#include "utils/rendercontext.hpp"
#include "utils/ltexture.hpp"

struct SDL_Renderer;

class GUIComponent;

class GUIView
{
    public:
        GUIView(const RenderContext& renderContext);

        bool init();

        void drawGUIComponent(const GUIComponent& component);
        void drawText(const int posX, const int posY, const float size, const std::string& text);

    private:
        const RenderContext& renderContext;

        LTexture asciiGrey;
};
