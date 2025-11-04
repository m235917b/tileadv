#include "resources/guicontextresource.hpp"

#include "ecs/ecs.hpp"

void initGUIContextResource(ECSContext &ecsContext,
                            const GUIContext &guiContext) {
  ecsContext.getCommandBuffer().upsertResource(GUIContextResource{&guiContext});
}
