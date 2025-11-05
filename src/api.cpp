#include "api.hpp"

#include "engine/engine.hpp"
#include "engine/resources.hpp"

void GameAPI::run() {
  asc.getECSContext().getScheduler().bootstrap();
  asc.run();
}

void GameAPI::registerEntityEffect(std::string id, EntityEffectFn effect) {
  auto table{*asc.getECSContext()
                  .getResourceManager()
                  .getResource<EntityEffectTableResource>()};

  table.table[id] = EntityEffect{id, std::move(effect)};

  asc.getECSContext().getCommandBuffer().upsertResource(std::move(table));
}
