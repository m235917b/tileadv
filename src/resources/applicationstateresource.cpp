#include "resources/applicationstateresource.hpp"

#include <any>

#include "ecs/ecs.hpp"

void initApplicationStateResource(ECSContext &ecsContext) {
  const auto appState{std::make_any<ApplicationStateResource>(
      ApplicationStateResource{ApplicationState::RUNNING})};
  const auto resAny{UpsertResource{appState}};
  ecsContext.getCommandBuffer().enqueue<UpsertResource>(resAny);
}
