#include "systems/spawnplayersystem.hpp"

#include "ecs/ecs.hpp"
#include "resources/playeridresource.hpp"

void registerSpawnPlayerSystem(const std::string &phase, ECSContext &ecsContext,
                               ECSAPI &ecsApi) {
  ecsContext.getScheduler().registerSystem(
      phase, "spawn_player", [&ecsApi](ECSContext &ecsContext, const float) {
        ecsApi.instantiateEntity("player", "player").finish();
        const auto id{
            std::make_any<PlayerIDResource>(PlayerIDResource{"player"})};
        const auto idAny{UpsertResource{id}};
        ecsContext.getCommandBuffer().enqueue<UpsertResource>(idAny);
      });
}
