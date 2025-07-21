#pragma once

enum class TileType
{
    EMPTY = 0,
    GRASS = 1,
    SAND = 2,
    COBBLESTONE = 3,
    SWAMP = 4,
    BRICKWALL = 100,
    WOODWALL = 101,
    STONEWALL = 102
};

struct Tile
{
    TileType type;
    bool solid{ false };
};

class TileFactory
{
    public:
        static Tile fromInt(int type);
};
