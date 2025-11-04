#include "player/resources.hpp"

#include "ecs/ecs.hpp"

void initPlayerIDResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(PlayerIDResource{});
}

void initPlayerResources(ECSContext &ecsContext) {
  initPlayerIDResource(ecsContext);
}
