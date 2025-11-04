#include "player/systems.hpp"
#include "actor/components.hpp"
#include "asc/resources.hpp"
#include "events/moveIntentEvent.hpp"
#include "player/resources.hpp"

const auto movementSystem{[](ECSContext &ecsContext, const float dt) {
  const auto &keyMap{
      ecsContext.getResourceManager().getResource<KeyMapResource>()};
  const auto &playerId{
      ecsContext.getResourceManager().getResource<PlayerIDResource>()};
  auto playerSpeedControl{
      *ecsContext.getStore().getComponent<SpeedControl>(playerId->id)};

  playerSpeedControl.timeLeft -= dt;

  if (playerSpeedControl.timeLeft <= 0) {
    if (keyMap->w) {
      playerSpeedControl.timeLeft = 1000 / playerSpeedControl.speed;
      ecsContext.getEventBus().publish<MoveIntentEvent>(
          MoveIntentEvent{playerId->id, Direction::UP});
    }
    if (keyMap->a) {
      playerSpeedControl.timeLeft = 1000 / playerSpeedControl.speed;
      ecsContext.getEventBus().publish<MoveIntentEvent>(
          MoveIntentEvent{playerId->id, Direction::LEFT});
    }
    if (keyMap->s) {
      playerSpeedControl.timeLeft = 1000 / playerSpeedControl.speed;
      ecsContext.getEventBus().publish<MoveIntentEvent>(
          MoveIntentEvent{playerId->id, Direction::DOWN});
    }
    if (keyMap->d) {
      playerSpeedControl.timeLeft = 1000 / playerSpeedControl.speed;
      ecsContext.getEventBus().publish<MoveIntentEvent>(
          MoveIntentEvent{playerId->id, Direction::RIGHT});
    }
  }

  ecsContext.getCommandBuffer().upsertComponent(playerId->id,
                                                playerSpeedControl);
}};

const auto spawnPlayerSystem{[](ECSContext &ecsContext, const float) {
  auto &ecsApi{
      *ecsContext.getResourceManager().getResource<ECSAPIResource>()->ecsApi};
  ecsApi.instantiateEntity("player", "player").finish();
  ecsContext.getCommandBuffer().upsertResource(PlayerIDResource{"player"});
}};

std::vector<SystemRegEntry> getPlayerSystems() {
  return {SystemRegEntry{"player_movement", movementSystem},
          SystemRegEntry{"player_spawn", spawnPlayerSystem}};
}
