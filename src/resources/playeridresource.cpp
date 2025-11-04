#include "resources/playeridresource.hpp"

#include "ecs/ecs.hpp"

void initPlayerIDResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(PlayerIDResource{});
}
