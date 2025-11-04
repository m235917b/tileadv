#pragma once

#include <string>

struct MoveIntentEvent {
  std::string entityId;
  int up;
  int left;
};
