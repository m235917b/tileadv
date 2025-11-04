#include "prefabs/playerprefab.hpp"

#include "ecs/ecs.hpp"

void registerPlayerPrefab(ECSAPI &ecsApi) {
  ecsApi.getPrefab().registerRecipe("player", playerActorPrefab,
                                    playerPositionPrefab, playerCharAttributes,
                                    playerMoveCooldown);
}
