#pragma once

#include <vector>

#include "model/tile.hpp"

class Chunk
{
    public:
        Chunk(const int sizex, const int sizey);

        const bool setTile(const int posx, const int posy, const Tile tile);

        const Tile& getTile(const int posx, const int posy) const;
        const int getWidth() const;
        const int getHeight() const;

    private:
        int sizex;
        int sizey;
        std::vector<Tile> worldGrid;
};
