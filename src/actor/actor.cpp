#include "actor/actor.hpp"

#include "actor/commands.hpp"
#include "actor/events.hpp"
#include "actor/resources.hpp"

void initActors(ECSContext &ecsContext) {
  initActorResources(ecsContext);
  initActorEvents(ecsContext);
  initActorCommands(ecsContext);
}
