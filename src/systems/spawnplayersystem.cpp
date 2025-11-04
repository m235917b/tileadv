#include "systems/spawnplayersystem.hpp"

#include "ecs/ecs.hpp"
#include "resources/playeridresource.hpp"

void registerSpawnPlayerSystem(const std::string &phase, ECSContext &ecsContext,
                               ECSAPI &ecsApi) {
  ecsContext.getScheduler().registerSystem(
      phase, "spawn_player", [&ecsApi](ECSContext &ecsContext, const float) {
        ecsApi.instantiateEntity("player", "player").finish();
        ecsContext.getCommandBuffer().upsertResource(
            PlayerIDResource{"player"});
      });
}
