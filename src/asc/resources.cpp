#include "asc/resources.hpp"

#include "ecs/ecs.hpp"

void initECSAPIResource(ECSContext &ecsContext, ECSAPI &ecsApi) {
  ecsContext.getCommandBuffer().upsertResource(ECSAPIResource{&ecsApi});
}

void initApplicationStateResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(
      ApplicationStateResource{ApplicationState::RUNNING});
}

void initKeyMapResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(
      KeyMapResource{false, false, false, false});
}

void initASCResources(ECSContext &ecsContext, ECSAPI &ecsApi) {
  initECSAPIResource(ecsContext, ecsApi);
  initApplicationStateResource(ecsContext);
  initKeyMapResource(ecsContext);
}
