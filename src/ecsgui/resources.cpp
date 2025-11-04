#include "ecsgui/resources.hpp"

#include "ecs/ecs.hpp"

void initGUIContextResource(ECSContext &ecsContext,
                            const GUIContext &guiContext) {
  ecsContext.getCommandBuffer().upsertResource(GUIContextResource{&guiContext});
}

void initECSGUIResources(ECSContext &ecsContext, GUIContext &guiContext) {
  initGUIContextResource(ecsContext, guiContext);
}
