#include "player/prefabs.hpp"

#include "ecs/ecs.hpp"

void registerPlayerPrefab(ECSAPI &ecsApi) {
  ecsApi.getPrefab().registerRecipe("player", playerActorPrefab,
                                    playerPositionPrefab, playerSpeedControl);
}

void initPlayerPrefabs(ECSAPI &ecsApi) { registerPlayerPrefab(ecsApi); }
