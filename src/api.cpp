#include "api.hpp"

#include <typeindex>

#include "engine/engine.hpp"
#include "engine/resources.hpp"

GameAPI::GameAPI() : systemFnId(0), entityId(0) {}

void GameAPI::run() {
  asc.getECSContext().getScheduler().bootstrap();
  asc.run();
}

void GameAPI::registerEntityEffect(std::string id, EntityEffectFn effect) {
  asc.getECSContext().getCommandBuffer().patchResource(
      std::type_index(typeid(EntityEffectTableResource)),
      [id = std::move(id), effect = std::move(effect)](std::any &res) {
        auto &table{std::any_cast<EntityEffectTableResource &>(res)};
        table.table[id] = EntityEffect{id, std::move(effect)};
      });
}

void GameAPI::registerEventEffectTrigger(std::type_index eventType,
                                         std::string effectId,
                                         EventPayloadAnyFn payload) {
  asc.getECSContext().getEventBus().subscribe(
      eventType, [effectId = std::move(effectId), payload = std::move(payload),
                  this](ECSContext &ecsContext, const std::any &event) {
        auto payloadRet{payload(event)};

        if (!payloadRet.has_value()) {
          return;
        }

        const auto &table{ecsContext.getResourceManager()
                              .getResource<EntityEffectTableResource>()
                              ->table};
        const auto it{table.find(effectId)};

        if (it == table.end()) {
          return;
        }

        it->second.apply(*this, payloadRet.value());
      });
}

void GameAPI::registerUpdate(std::vector<std::type_index> types,
                             UpdateAny update) {
  const auto wrapper{[update = std::move(update),
                      this](ECSContext &, const float dt,
                            const std::string &entityId,
                            const std::vector<const std::any *> &components) {
    update(*this, dt, entityId, components);
  }};

  asc.getECSAPI().addViewSystem("logic",
                                "apiSys_" + std::to_string(systemFnId++),
                                std::move(types), std::move(wrapper));
}

void GameAPI::print(std::string text) {
  asc.getECSContext().getCommandBuffer().print(std::move(text));
}

ECSPrefab &GameAPI::getPrefab() { return asc.getECSAPI().getPrefab(); }

/*ECSEntityBuilder GameAPI::createEntity(std::string entityId) {
  return asc.getECSAPI().createEntity(std::move(entityId));
}*/

ECSEntityBuilder GameAPI::instantiateEntity(std::string recipeId) {
  return asc.getECSAPI().instantiateEntity(
      "user_entity_" + std::to_string(entityId++), std::move(recipeId));
}

void GameAPI::upsertComponent(std::string entityId, std::any component) {
  asc.getECSContext().getCommandBuffer().upsertComponent(std::move(entityId),
                                                         std::move(component));
}

const std::any *GameAPI::getComponent(const std::string &entityId,
                                      const std::type_index &type) {
  return asc.getECSContext().getStore().getComponent(entityId, type);
}
