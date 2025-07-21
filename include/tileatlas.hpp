#include "tiletypes.hpp"

struct Tile;
struct SDL_FRect;

constexpr float tileSpriteSize{ 25.f };

class TileAtlas
{
    public:
        static Tile fromInt(int type);
        static SDL_FRect getSpriteCoords(TileType type);
};