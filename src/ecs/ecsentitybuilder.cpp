#include "ecs/ecsentitybuilder.hpp"
#include "ecs/ecscontext.hpp"

ECSEntityBuilder::ECSEntityBuilder(ECSContext &context, ECSPrefab &prefab,
                                   std::string entityId)
    : context(context), prefab(prefab), entityId(std::move(entityId)),
      recipeName(""), components() {};

ECSEntityBuilder::ECSEntityBuilder(ECSContext &context, ECSPrefab &prefab,
                                   std::string entityId, std::string recipeName)
    : context(context), prefab(prefab), entityId(std::move(entityId)),
      recipeName(std::move(recipeName)), components() {};

ECSEntityBuilder &ECSEntityBuilder::add(std::any component) {
  components[std::type_index(component.type())] = std::move(component);

  return *this;
}

void ECSEntityBuilder::finish() {
  for (auto &[type, component] : components) {
    context.getCommandBuffer().enqueue<UpsertComponent>(
        UpsertComponent{entityId, std::move(component)});
  }
}
