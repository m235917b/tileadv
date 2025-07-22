#include "model/tileactor.hpp"
#include "model/chunk.hpp"

TileActor::TileActor(const int posx, const int posy)
    : Actor(), posx(posx), posy(posy)
{

}

void TileActor::move(const int dx, const int dy, const Chunk& chunk)
{
    if (posx + dx < 0 || posx + dx >= chunk.getWidth() || posy + dy < 0 || posy + dy >= chunk.getHeight())
    {
        return;
    }

    if (chunk.getTile(posx + dx, posy + dy).solid)
    {
        return;
    }

    posx += dx;
    posy += dy;
}

const int TileActor::getPosX() const
{
    return posx;
}

const int TileActor::getPosY() const
{
    return posy;
}
