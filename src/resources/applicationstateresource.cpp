#include "resources/applicationstateresource.hpp"

#include <any>

#include "ecs/ecs.hpp"

void initApplicationStateResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(
      ApplicationStateResource{ApplicationState::RUNNING});
}
