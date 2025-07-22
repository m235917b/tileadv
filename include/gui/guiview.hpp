#pragma once

#include "utils/ltexture.hpp"

struct SDL_Renderer;

class GUIMenu;
class GUIContainer;
class GUIElement;

class GUIView
{
    public:
        GUIView();

        bool init(SDL_Renderer* renderer);
        void destroy();

        void drawGUIMenu(SDL_Renderer* renderer, const GUIMenu& menu);
        void drawVerticalLayout(SDL_Renderer* renderer, const int posX, const int posY, const GUIContainer& container);
        void drawTextElement(SDL_Renderer* renderer, const int posX, const int posY, const GUIElement& element);
        void drawText(SDL_Renderer* renderer, const int posX, const int posY, const float size, const std::string& text);

    private:
        LTexture asciiGrey;
};
