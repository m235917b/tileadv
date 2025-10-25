#include <cmath>
#include <memory>

#include "gamecontroller.hpp"
#include "model/character.hpp"
#include "model/fireball.hpp"
#include "model/fireprojectile.hpp"
#include "utils/math.hpp"

Fireball::Fireball() : Item() {}

void Fireball::use(Character &character, const int targetX, const int targetY,
                   GameController &gameController) {
  const auto path =
      bresenham(character.getPosX(), character.getPosY(), targetX, targetY);

  gameController.addTileActor(std::make_unique<FireProjectile>(path));
}
