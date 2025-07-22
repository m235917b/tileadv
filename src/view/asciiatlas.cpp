#include <SDL3/SDL.h>

#include "view/asciiatlas.hpp"

SDL_FRect AsciiAtlas::getSpriteCoords(char asciiChar)
{
    SDL_FRect rect;

    // Calculate the sprite coordinates based on the ASCII character
    rect.x = (asciiChar - 32) * asciiWidth;
    rect.y = 0;
    rect.w = asciiWidth;
    rect.h = asciiHeight;

    return rect;
}
