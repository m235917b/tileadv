#pragma once

#include <vector>

#include "model/item.hpp"
#include "model/tileactor.hpp"

class GameController;

class Character : public TileActor {
public:
  Character(const int x, const int y, const TileActorType type);

  void setHealth(const int health);
  void addItem(std::unique_ptr<Item> item);

  int getHealth() const;

  void useActionItem(const int targetX, const int targetY,
                     GameController &gameController);

  void equipActionItem(Item *item);

private:
  int health;

  std::vector<std::unique_ptr<Item>> inventory;
  std::unique_ptr<Item> actionItem;
};
