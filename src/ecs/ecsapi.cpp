#include "ecs/ecsapi.hpp"
#include "ecs/ecscontext.hpp"
#include "ecs/ecsentitybuilder.hpp"

ECSAPI::ECSAPI(ECSContext &context) : context(context), prefab() {}

ECSPrefab &ECSAPI::getPrefab() { return prefab; }

ECSEntityBuilder ECSAPI::createEntity(std::string entityId) {
  return ECSEntityBuilder(context, prefab, std::move(entityId));
}

ECSEntityBuilder ECSAPI::instantiateEntity(std::string entityId,
                                           std::string recipeId) {
  return ECSEntityBuilder(context, prefab, std::move(entityId),
                          std::move(recipeId));
}

void ECSAPI::addViewSystem(
    const std::string &phase, std::string systemId,
    std::vector<std::type_index> types,
    std::function<void(ECSContext &, const float, const std::string &,
                       const std::vector<const std::any *> &)>
        system) {
  const auto sysFunc{
      [this, types = std::move(types),
       system = std::move(system)](ECSContext &context, const float dt) {
        const auto sysFuncView{
            [&](const std::string &entityId,
                const std::vector<const std::any *> &components) {
              system(context, dt, entityId, components);
            }};
        this->context.getStore().view(types, sysFuncView);
      }};

  context.getScheduler().addSystem(phase, systemId, sysFunc);
}

void ECSAPI::subscribeViewEventListener(
    const std::type_index &type, std::vector<std::type_index> types,
    std::function<void(ECSContext &, const std::any &, const std::string &,
                       const std::vector<const std::any *> &)>
        listener) {
  const auto listenerFunc{
      [this, types = std::move(types), listener = std::move(listener)](
          ECSContext &context, const std::any &event) {
        const auto listenerView{
            [&](const std::string &entityId,
                const std::vector<const std::any *> &components) {
              listener(context, event, entityId, components);
            }};
        this->context.getStore().view(types, listenerView);
      }};

  context.getEventBus().subscribe(type, listenerFunc);
}
