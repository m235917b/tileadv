#include <SDL3/SDL.h>

#include "view/tileatlas.hpp"

const SDL_FRect TileAtlas::getTileCoords(const TileType type) {
  const auto xFactor{static_cast<int>(type) % 100};
  const auto yFactor{static_cast<int>(static_cast<int>(type) / 100)};

  return {xFactor * tileSpriteSize, yFactor * tileSpriteSize, tileSpriteSize,
          tileSpriteSize};
}

const SDL_FRect TileAtlas::getActorCoords(const TileActorType type) {
  const auto xFactor{static_cast<int>(type)};

  return {xFactor * tileSpriteSize, 0.f, tileSpriteSize, tileSpriteSize};
}
