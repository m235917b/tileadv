#include <SDL3/SDL.h>

#include "view/tileatlas.hpp"

const SDL_FRect TileAtlas::getSpriteCoords(const TileType type) {
  int xFactor{static_cast<int>(type) % 100};
  int yFactor{static_cast<int>(static_cast<int>(type) / 100)};

  return {xFactor * tileSpriteSize, yFactor * tileSpriteSize, tileSpriteSize,
          tileSpriteSize};
}
