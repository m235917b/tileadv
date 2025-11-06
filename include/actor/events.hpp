#pragma once

#include <string>

class ECSContext;

enum class Direction { UP, LEFT, DOWN, RIGHT };

struct MoveIntentEvent {
  std::string entityId;
  Direction dir;
};

struct WorldCollisionEvent {
  std::string entityId;
};

struct ActorCollisionEvent {
  std::string moved;
  std::string other;
};

void initActorEvents(ECSContext &ecsContext);
