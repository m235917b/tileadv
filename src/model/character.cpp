#include "model/character.hpp"
#include "gamecontroller.hpp"
#include "model/item.hpp"

Character::Character(const int x, const int y, const TileActorType type)
    : TileActor(x, y, type), health(100) {}

void Character::setHealth(const int health) { this->health = health; }

void Character::addItem(Item *item) { inventory.push_back(item); }

int Character::getHealth() const { return health; }

void Character::useActionItem(const int targetX, const int targetY,
                              GameController &gameController) {
  if (actionItem) {
    actionItem->use(*this, targetX, targetY, gameController);
  }
}

void Character::equipActionItem(Item *item) { actionItem = item; }
