#include "ecs/ecsentitybuilder.hpp"
#include "ecs/ecscontext.hpp"

ECSEntityBuilder::ECSEntityBuilder(ECSContext &context, ECSPrefab &prefab,
                                   std::string entityId)
    : context(context), prefab(prefab), entityId(std::move(entityId)),
      recipeName(""), components() {};

ECSEntityBuilder::ECSEntityBuilder(ECSContext &context, ECSPrefab &prefab,
                                   std::string entityId, std::string recipeId)
    : context(context), prefab(prefab), entityId(std::move(entityId)),
      recipeName(std::move(recipeId)), components() {
  const auto *seeds{prefab.getRecipeDefaults(recipeName)};

  if (!seeds) {
    return;
  }

  for (const auto &seed : *seeds) {
    add(seed);
  }
};

ECSEntityBuilder &ECSEntityBuilder::add(std::any component) {
  components[std::type_index(component.type())] = std::move(component);

  return *this;
}

ECSEntityBuilder &ECSEntityBuilder::add(std::type_index componentType) {
  const auto *seed{prefab.getDefault(componentType)};

  if (!seed) {
    return *this;
  }

  add(*seed);

  return *this;
}

void ECSEntityBuilder::finish() {
  for (auto &[type, component] : components) {
    context.getCommandBuffer().enqueue<UpsertComponent>(
        UpsertComponent{entityId, std::move(component)});
  }
}
