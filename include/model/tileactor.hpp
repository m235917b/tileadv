#pragma once

#include "model/actor.hpp"

class Chunk;

class TileActor : public Actor
{
    public:
        TileActor(int posx, int posy);

        void move(int dx, int dy, const Chunk& chunk);

        int getPosX() const;
        int getPosY() const;

    private:
        int posx;
        int posy;
};
