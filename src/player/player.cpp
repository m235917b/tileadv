#include "player/player.hpp"

#include "player/prefabs.hpp"
#include "player/resources.hpp"

void initPlayer(ECSContext &ecsContext, ECSAPI &ecsApi) {
  initPlayerResources(ecsContext);
  initPlayerPrefabs(ecsApi);
}
