#pragma once

#include <algorithm>
#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

class ECSEntityBuilder;

class ECSPrefab {
public:
  ECSPrefab() = default;
  ~ECSPrefab() = default;

  void registerRecipe(std::string recipeId,
                      std::vector<std::any> defaultComponents);
  void registerRecipe(std::type_index componentType, std::any defaultComponent);
  const std::any *getRecipeDefault(const std::type_index &componentType,
                                   const std::string &recipeId) const;
  const std::any *getDefault(const std::type_index &componentType) const;

  template <typename... Ts>
  void registerRecipe(std::string recipeId, Ts... defaultComponents) {
    std::vector<std::any> vec;
    vec.reserve(sizeof...(Ts));
    (vec.emplace_back(std::forward<Ts>(defaultComponents)), ...);
    registerRecipe(std::move(recipeId), std::move(vec));
  }

  template <typename ComponentType>
  void registerRecipe(ComponentType defaultComponent) {
    registerRecipe(std::type_index(typeid(ComponentType)),
                   std::make_any<ComponentType>(std::move(defaultComponent)));
  }

  template <typename ComponentType>
  const ComponentType *getRecipeDefault(const std::string &recipeId) const {
    const std::any *anyRec{
        getRecipeDefault(std::type_index(typeid(ComponentType)), recipeId)};
    return anyRec ? &std::any_cast<const ComponentType &>(*anyRec) : nullptr;
  }

  template <typename ComponentType> const ComponentType *getDefault() const {
    const std::any *anyRec{getDefault(std::type_index(typeid(ComponentType)))};
    return anyRec ? &std::any_cast<const ComponentType &>(*anyRec) : nullptr;
  }

private:
  std::unordered_map<std::string, std::vector<std::any>> recipes;
  std::unordered_map<std::type_index, std::any> defaults;

  const std::vector<std::any> *
  getRecipeDefaults(const std::string &recipeId) const;

  friend class ECSEntityBuilder;
};
