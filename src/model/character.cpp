#include <algorithm>

#include "gamecontroller.hpp"
#include "model/character.hpp"
#include "model/item.hpp"

Character::Character(const int x, const int y, const TileActorType type)
    : TileActor(x, y, type, 3), health(100) {}

void Character::setHealth(const int health) { this->health = health; }

void Character::addItem(std::unique_ptr<Item> item) {
  inventory.push_back(std::move(item));
}

int Character::getHealth() const { return health; }

void Character::useActionItem(const int targetX, const int targetY,
                              GameController &gameController) {
  if (actionItem) {
    actionItem->use(*this, targetX, targetY, gameController);
  }
}

void Character::equipActionItem(Item *item) {
  auto it = std::find_if(inventory.begin(), inventory.end(),
                         [&](const std::unique_ptr<Item> &invItem) {
                           return invItem.get() == item;
                         });
  if (it != inventory.end()) {
    actionItem = std::move(*it);
    inventory.erase(it);
  }
}
