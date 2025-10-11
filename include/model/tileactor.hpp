#pragma once

#include "model/actor.hpp"

class Chunk;

enum class TileActorType { PLAYER, DUMMY };

class TileActor : public Actor {
public:
  TileActor(const int posX, const int posY, const TileActorType type);
  ~TileActor() = default;

  void move(const float dx, const float dy, const Chunk &chunk);

  int getPosX() const;
  int getPosY() const;
  float getRealPosX() const;
  float getRealPosY() const;
  TileActorType getType() const;

  void setPosX(const int posX);
  void setPosY(const int posY);

protected:
  float posX;
  float posY;

private:
  float speed;

  TileActorType type;
};
