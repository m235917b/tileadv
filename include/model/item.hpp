#pragma once

#include <memory>
#include <vector>

class Character;
class GameController;

class Item {
public:
  Item();
  virtual ~Item() = default;

  virtual void use(Character &character, const int targetX, const int targetY,
                   GameController &gameController) = 0;
};
