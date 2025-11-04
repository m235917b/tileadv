#pragma once

#include <string>

enum class Direction { UP, LEFT, DOWN, RIGHT };

struct MoveIntentEvent {
  std::string entityId;
  Direction dir;
};
