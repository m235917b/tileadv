#include "actor/commands.hpp"

#include "actor/components.hpp"
#include "asc/resources.hpp"
#include "ecs/ecs.hpp"

void registerPopulateChunkCommandHandler(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().registerHandler(
      std::type_index(typeid(PopulateChunk)),
      [](ECSContext &context, const std::any &command) {
        auto id{std::any_cast<PopulateChunk>(command).chunkId};
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

void initActorCommands(ECSContext &ecsContext) {
  registerPopulateChunkCommandHandler(ecsContext);
}
