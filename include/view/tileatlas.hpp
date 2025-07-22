#pragma once

#include "model/tile.hpp"

struct SDL_FRect;

constexpr float tileSpriteSize{ 25.f };

class TileAtlas
{
    public:
        static SDL_FRect getSpriteCoords(TileType type);
};