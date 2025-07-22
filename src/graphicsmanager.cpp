#include <SDL3/SDL.h>

#include "graphicsmanager.hpp"

GraphicsManmager::GraphicsManmager()
    : screenWidth(2560), screenHeight(1440), window(nullptr), renderer(nullptr)
{

}

bool GraphicsManmager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_CreateWindowAndRenderer("TileADV", this->screenWidth, this->screenHeight, 0, &this->window, &this->renderer) == false)
    {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_SetWindowFullscreen(this->window, true) == false)
    {
        SDL_Log("Could not set fullscreen mode! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_HideCursor() == false)
    {
        SDL_Log("Could not hide cursor! SDL error: %s\n", SDL_GetError());
        return false;
    }

    /*
    if(SDL_SetRenderVSync(renderer, 1) == false)
    {
        SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
        success = false;
    }
    */

    return true;
}

int GraphicsManmager::destroy()
{
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
    SDL_DestroyWindow(this->window);
    this->window = nullptr;

    SDL_Quit();

    return 0;
}

void GraphicsManmager::beginFrame()
{
    SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(this->renderer);
}

void GraphicsManmager::endFrame()
{
    SDL_RenderPresent(this->renderer);
}

const int GraphicsManmager::getScreenWidth() const
{
    return this->screenWidth;
}

const int GraphicsManmager::getScreenHight() const
{
    return this->screenHeight;
}

SDL_Renderer* GraphicsManmager::getRenderer() const
{
    return this->renderer;
}
