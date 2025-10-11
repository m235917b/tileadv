#pragma once

#include <vector>

#include "model/tileactor.hpp"

class Item;
class GameController;

class Character : public TileActor {
public:
  Character(const int x, const int y, const TileActorType type);

  void setHealth(const int health);
  void addItem(Item *item);

  int getHealth() const;

  void useActionItem(const int targetX, const int targetY,
                     GameController &gameController);

  void equipActionItem(Item *item);

private:
  int health;

  std::vector<Item *> inventory;
  Item *actionItem;
};
