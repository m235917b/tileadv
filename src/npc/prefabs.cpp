#include "npc/prefabs.hpp"

#include "actor/components.hpp"
#include "ecs/ecs.hpp"

void registerDummyPrefab(ECSAPI &ecsApi) {
  ecsApi.getPrefab().registerRecipe("dummy", Actor{ActorType::DUMMY},
                                    Position{10, 10}, Physics{true});
}

void initNPCPrefabs(ECSAPI &ecsApi) { registerDummyPrefab(ecsApi); }
