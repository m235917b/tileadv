#include "player/prefabs.hpp"

#include "ecs/ecs.hpp"

void registerPlayerPrefab(ECSAPI &ecsApi) {
  ecsApi.getPrefab().registerRecipe("player", playerActorPrefab,
                                    playerPositionPrefab, playerSpeedControl,
                                    PersistComponent{}, Physics{true});
}

void initPlayerPrefabs(ECSAPI &ecsApi) { registerPlayerPrefab(ecsApi); }
