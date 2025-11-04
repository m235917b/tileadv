#include "resources/rendercontextresource.hpp"

#include "ecs/ecs.hpp"

void initRenderContextResource(ECSContext &ecsContext,
                               const RenderContext &renderContext) {
  ecsContext.getCommandBuffer().upsertResource(
      RenderContextResource{&renderContext});
}