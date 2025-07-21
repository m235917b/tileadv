#include <SDL3/SDL.h>

#include "tileatlas.hpp"
#include "tiletypes.hpp"
#include "tile.hpp"

Tile TileAtlas::fromInt(int type)
{
    Tile tile;

    tile.type = static_cast<TileType>(type);

    if(type >= 100)
    {
        tile.solid = true;
    }

    return tile;
}

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
