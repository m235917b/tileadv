#include "model/chunk.hpp"
#include "model/tile.hpp"

Chunk::Chunk(int sizex, int sizey):
sizex{sizex},
sizey{sizey}
{
    worldGrid = std::vector<Tile>(sizex * sizey, {TileType::EMPTY});
}

bool Chunk::setTile(int posx, int posy, Tile tile)
{
    if(posx < 0 || posx >= sizex || posy < 0 || posy >= sizey)
    {
        return false;
    }

    worldGrid[posy * sizex + posx] = tile;

    return true;
}

const Tile& Chunk::getTile(int posx, int posy) const
{
    return worldGrid[posy * sizex + posx];
}

int Chunk::getWidth() const
{
    return sizex;
}

int Chunk::getHeight() const
{
    return sizey;
}
