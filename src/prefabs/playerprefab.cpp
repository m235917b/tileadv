#include "prefabs/playerprefab.hpp"

#include "ecs/ecs.hpp"

void registerPlayerPrefab(ECSAPI &ecsApi) {
  ecsApi.getPrefab().registerRecipe<Actor>("player", playerActorPrefab);
  ecsApi.getPrefab().registerRecipe<Position>("player", playerPositionPrefab);
}
