#pragma once

#include <vector>

#include "model/tile.hpp"

class Chunk
{
    public:
        Chunk(int sizex, int sizey);

        bool setTile(int posx, int posy, Tile tile);

        const Tile& getTile(int posx, int posy) const;
        int getWidth() const;
        int getHeight() const;

    private:
        int sizex;
        int sizey;
        std::vector<Tile> worldGrid;
};
