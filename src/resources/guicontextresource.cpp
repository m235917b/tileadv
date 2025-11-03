#include "resources/guicontextresource.hpp"

#include "ecs/ecs.hpp"

void initGUIContextResource(ECSContext &ecsContext,
                            const GUIContext &guiContext) {
  const auto guic{
      std::make_any<GUIContextResource>(GUIContextResource{&guiContext})};
  const auto guicAny{UpsertResource{guic}};
  ecsContext.getCommandBuffer().enqueue<UpsertResource>(guicAny);
}
