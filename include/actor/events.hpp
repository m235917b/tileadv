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

void initActorEvents(ECSContext &ecsContext);
