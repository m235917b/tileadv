#include "model/fireprojectile.hpp"

FireProjectile::FireProjectile(const int posX, const int posY, const float dx,
                               const float dy)
    : TileActor(posX, posY, TileActorType::DUMMY), dx(dx), dy(dy) {}

void FireProjectile::update() {
  posX += dx;
  posY += dy;
}
