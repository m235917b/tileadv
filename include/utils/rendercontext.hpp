#pragma once

struct SDL_Renderer;

class RenderContext
{
    public:
        virtual SDL_Renderer* getRenderer() const = 0;
        virtual const int getScreenWidth() const = 0;
        virtual const int getScreenHeight() const = 0;
};
