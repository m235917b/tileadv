#include <cmath>
#include <memory>

#include "gamecontroller.hpp"
#include "model/character.hpp"
#include "model/fireball.hpp"
#include "model/fireprojectile.hpp"

Fireball::Fireball() : Item() {}

void Fireball::use(Character &character, const int targetX, const int targetY,
                   GameController &gameController) {
  const float dx = static_cast<float>(targetX - character.getPosX());
  const float dy = static_cast<float>(targetY - character.getPosY());
  const float length = std::sqrt(dx * dx + dy * dy);
  const float ndx = (length != 0) ? (dx / length) * 1.f : 0;
  const float ndy = (length != 0) ? (dy / length) * 1.f : 0;

  gameController.addTileActor(std::make_unique<FireProjectile>(
      character.getPosX(), character.getPosY(), ndx, ndy));
}
