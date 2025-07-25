#pragma once

#include "model/actor.hpp"

class Chunk;

class TileActor : public Actor {
public:
  TileActor(const int posx, const int posy);

  void move(const int dx, const int dy, const Chunk &chunk);

  int getPosX() const;
  int getPosY() const;

private:
  int posx;
  int posy;
};
