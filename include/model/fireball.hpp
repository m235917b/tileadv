#pragma once

#include "model/item.hpp"

class Character;
class GameController;

class Fireball : public Item {
public:
  Fireball();
  ~Fireball() = default;

  void use(Character &character, int targetX, int targetY,
           GameController &gameController) override;
};
