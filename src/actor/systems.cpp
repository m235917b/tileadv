#include "actor/systems.hpp"

#include "actor/components.hpp"

const auto garbageCollectorSystem{[](ECSContext &ecsContext, const float) {
  ecsContext.getStore().view<Garbage>(
      [&ecsContext](const std::string &entityId, const Garbage &garb) {
        if (garb.isGarbage) {
          ecsContext.getCommandBuffer().destroyEntity(entityId);
        }
      });
}};

std::vector<SystemRegEntry> getActorSystems() {
  return {SystemRegEntry{"garbage_collector", garbageCollectorSystem}};
}
