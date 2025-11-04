#include "systems/movementsystem.hpp"

#include "components/characterattributes.hpp"
#include "components/movecooldown.hpp"
#include "ecs/ecs.hpp"
#include "events/moveIntentEvent.hpp"
#include "resources/keymapresource.hpp"
#include "resources/playeridresource.hpp"

std::string registerMovementSystem(const std::string &phase,
                                   ECSContext &ecsContext) {
  const std::string id{"movement"};

  ecsContext.getScheduler().registerSystem(
      phase, id, [](ECSContext &ecsContext, const float dt) {
        const auto &keyMap{
            ecsContext.getResourceManager().getResource<KeyMapResource>()};
        const auto &playerId{
            ecsContext.getResourceManager().getResource<PlayerIDResource>()};
        const auto &playerCharAttrib{
            ecsContext.getStore().getComponent<CharacterAttributes>(
                playerId->id)};
        auto playerMoveCooldown{
            *ecsContext.getStore().getComponent<MoveCooldown>(playerId->id)};

        playerMoveCooldown.timeLeft -= dt;

        if (playerMoveCooldown.timeLeft <= 0) {
          if (keyMap->w) {
            playerMoveCooldown.timeLeft = 1000 / playerCharAttrib->speed;
            ecsContext.getEventBus().publish<MoveIntentEvent>(
                MoveIntentEvent{playerId->id, Direction::UP});
          }
          if (keyMap->a) {
            playerMoveCooldown.timeLeft = 1000 / playerCharAttrib->speed;
            ecsContext.getEventBus().publish<MoveIntentEvent>(
                MoveIntentEvent{playerId->id, Direction::LEFT});
          }
          if (keyMap->s) {
            playerMoveCooldown.timeLeft = 1000 / playerCharAttrib->speed;
            ecsContext.getEventBus().publish<MoveIntentEvent>(
                MoveIntentEvent{playerId->id, Direction::DOWN});
          }
          if (keyMap->d) {
            playerMoveCooldown.timeLeft = 1000 / playerCharAttrib->speed;
            ecsContext.getEventBus().publish<MoveIntentEvent>(
                MoveIntentEvent{playerId->id, Direction::RIGHT});
          }
        }

        ecsContext.getCommandBuffer().upsertComponent(playerId->id,
                                                      playerMoveCooldown);
      });

  return id;
}
