#include "engine/resources.hpp"

#include "ecs/ecs.hpp"

void registerEntityEffectTableResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(EntityEffectTableResource{});
}

void initEngineResources(ECSContext &ecsContext) {
  registerEntityEffectTableResource(ecsContext);
}
