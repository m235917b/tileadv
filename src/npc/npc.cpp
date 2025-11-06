#include "npc/npc.hpp"

#include "ecs/ecs.hpp"
#include "npc/events.hpp"
#include "npc/systems.hpp"

void initNPCs(ECSContext &ecsContext, ECSAPI &ecsApi) {
  initNPCPrefabs(ecsApi);
  initNPCEvents(ecsContext);
}
