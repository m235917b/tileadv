#pragma once

#include "model/actor.hpp"

class Chunk;

class TileActor : public Actor {
public:
  TileActor(const int posX, const int posY);

  void move(const int dx, const int dy, const Chunk &chunk);

  int getPosX() const;
  int getPosY() const;

  void setPosX(const int posX);
  void setPosY(const int posY);

private:
  int posX;
  int posY;
};
