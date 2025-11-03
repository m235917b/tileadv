#include "resources/rendercontextresource.hpp"

#include "ecs/ecs.hpp"

void initRenderContextResource(ECSContext &ecsContext,
                               const RenderContext &renderContext) {
  const auto rc{std::make_any<RenderContextResource>(
      RenderContextResource{&renderContext})};
  const auto rcAny{UpsertResource{rc}};
  ecsContext.getCommandBuffer().enqueue<UpsertResource>(rcAny);
}