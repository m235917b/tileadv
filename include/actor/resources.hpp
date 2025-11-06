#pragma once

#include <vector>

#include "actor/events.hpp"

class ECSContext;

struct MoveIntentEventQueue {
  std::vector<MoveIntentEvent> moveIntents;
};

void initActorResources(ECSContext &ecsContext);
