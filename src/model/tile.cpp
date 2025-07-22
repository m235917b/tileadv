#include "model/tile.hpp"

Tile TileFactory::fromInt(const int type)
{
    Tile tile;

    tile.type = static_cast<TileType>(type);

    if(type >= 100)
    {
        tile.solid = true;
    }

    return tile;
}