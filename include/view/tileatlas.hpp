#pragma once

#include "model/tile.hpp"

struct SDL_FRect;

constexpr float tileSpriteSize{25.f};

class TileAtlas {
public:
  static const SDL_FRect getSpriteCoords(const TileType type);
};