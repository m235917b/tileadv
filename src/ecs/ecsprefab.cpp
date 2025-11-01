#include "ecs/ecsprefab.hpp"
#include "ecs/ecscontext.hpp"

void ECSPrefab::registerRecipe(std::string recipeId,
                               std::vector<std::any> defaultComponents) {
  recipes[std::move(recipeId)] = std::move(defaultComponents);
}

void ECSPrefab::registerRecipe(std::type_index componentType,
                               std::any defaultComponent) {
  defaults[componentType] = std::move(defaultComponent);
}

const std::any *
ECSPrefab::getRecipeDefault(const std::type_index &componentType,
                            const std::string &recipeId) const {
  const auto it{recipes.find(recipeId)};

  if (it == recipes.end()) {
    return nullptr;
  }

  const auto &components{it->second};
  const auto compIt{std::find_if(components.begin(), components.end(),
                                 [&componentType](const auto &component) {
                                   return std::type_index(component.type()) ==
                                          componentType;
                                 })};

  if (compIt == components.end()) {
    return nullptr;
  }

  return &(*compIt);
}

const std::any *
ECSPrefab::getDefault(const std::type_index &componentType) const {
  const auto it{defaults.find(componentType)};

  if (it == defaults.end()) {
    return nullptr;
  }

  return &(it->second);
}

const std::vector<std::any> *
ECSPrefab::getRecipeDefaults(const std::string &recipeId) const {
  const auto it{recipes.find(recipeId)};

  if (it == recipes.end()) {
    return nullptr;
  }

  return &(it->second);
}
