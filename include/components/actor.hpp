#pragma once

enum class ActorType { PLAYER, DUMMY, FIREBALL };

struct Actor {
  ActorType type;
};
