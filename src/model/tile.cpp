#include "model/tile.hpp"

Tile TileFactory::fromInt(const int type) {
  return {static_cast<TileType>(type), type >= 100};
}
