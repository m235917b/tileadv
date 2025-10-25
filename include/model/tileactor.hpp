#pragma once

#include <memory>
#include <vector>

#include "model/actor.hpp"
#include "utils/timer.hpp"

class Chunk;

enum class TileActorType { PLAYER, DUMMY, FIREBALL };

class TileActor : public Actor {
public:
  TileActor(const int posX, const int posY, const TileActorType type,
            const int layer);
  ~TileActor() = default;

  bool move(const int dx, const int dy, const Chunk &chunk);
  virtual void
  updateInWorld([[maybe_unused]] const Chunk &chunk,
                [[maybe_unused]] const std::vector<std::unique_ptr<TileActor>>
                    &tileActors);

  int getPosX() const;
  int getPosY() const;
  float getRealPosX() const;
  float getRealPosY() const;
  TileActorType getType() const;
  bool isAlive() const;
  int getLayer() const;

  void setPosX(const int posX);
  void setPosY(const int posY);

protected:
  float posX;
  float posY;
  bool alive;
  DeltaTimer speedTimer;

private:
  TileActorType type;
  int layer;
};
