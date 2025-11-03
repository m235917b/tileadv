#include "resources/playeridresource.hpp"

#include "ecs/ecs.hpp"

void initPlayerIDResource(ECSContext &ecsContext) {
  const auto id{std::make_any<PlayerIDResource>(PlayerIDResource{})};
  const auto idAny{UpsertResource{id}};
  ecsContext.getCommandBuffer().enqueue<UpsertResource>(idAny);
}
