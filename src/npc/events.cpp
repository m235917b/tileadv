#include "npc/events.hpp"

#include <typeindex>

#include "actor/components.hpp"
#include "asc/resources.hpp"
#include "chunk/resources.hpp"
#include "ecs/ecs.hpp"

void registerPopulateChunkNPCEventListener(ECSContext &ecsContext) {
  ecsContext.getEventBus().subscribe(
      std::type_index(typeid(PopulateChunkNPC)),
      [](ECSContext &context, const std::any &event) {
        auto id{std::any_cast<PopulateChunkNPC>(event).chunkId};
        if (id == "chunk1") {
          auto &ecsApi{*context.getResourceManager()
                            .getResource<ECSAPIResource>()
                            ->ecsApi};
          ecsApi.instantiateEntity("dummy1", "dummy")
              .set(&Position::x, 80)
              .set(&Position::y, 20)
              .finish();
          ecsApi.instantiateEntity("dummy2", "dummy")
              .set(&Position::x, 50)
              .set(&Position::y, 25)
              .finish();
          ecsApi.instantiateEntity("dummy3", "dummy")
              .set(&Position::x, 30)
              .set(&Position::y, 50)
              .finish();
        }
      });
}

void initNPCEvents(ECSContext &ecsContext) {
  registerPopulateChunkNPCEventListener(ecsContext);
}
