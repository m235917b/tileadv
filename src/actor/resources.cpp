#include "actor/resources.hpp"

#include "ecs/ecs.hpp"

void initMoveIntentEventQueueResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(
      std::make_any<MoveIntentEventQueue>(
          MoveIntentEventQueue{std::vector<MoveIntentEvent>{}}));
}

void initActorResources(ECSContext &ecsContext) {
  initMoveIntentEventQueueResource(ecsContext);
}
