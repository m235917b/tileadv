#pragma once

#include "model/tileactor.hpp"

class FireProjectile : public TileActor {
public:
  FireProjectile(const int posX, const int posY, const float dx,
                 const float dy);
  ~FireProjectile() = default;

  void update() override;

private:
  const float dx;
  const float dy;
};
