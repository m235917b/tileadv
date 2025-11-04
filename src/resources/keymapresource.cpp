#include "resources/keymapresource.hpp"

#include "ecs/ecs.hpp"

void initKeyMapResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(
      KeyMapResource{false, false, false, false});
}
