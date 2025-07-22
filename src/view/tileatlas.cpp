#include <SDL3/SDL.h>

#include "view/tileatlas.hpp"

SDL_FRect TileAtlas::getSpriteCoords(TileType type)
{
    SDL_FRect rect;

    int xFactor = static_cast<int>(type) % 100;
    int yFactor = static_cast<int>(static_cast<int>(type) / 100);

    rect.x = xFactor * tileSpriteSize;
    rect.y = yFactor * tileSpriteSize;
    rect.w = tileSpriteSize;
    rect.h = tileSpriteSize;

    return rect;
}
