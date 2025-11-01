#pragma once

#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "ecs/ecsprefab.hpp"

class ECSContext;

class ECSEntityBuilder {
public:
  ECSEntityBuilder(ECSContext &context, ECSPrefab &prefab,
                   std::string entityId);
  ECSEntityBuilder(ECSContext &context, ECSPrefab &prefab, std::string entityId,
                   std::string recipeName);
  ~ECSEntityBuilder() = default;

  ECSEntityBuilder &add(std::any component);
  ECSEntityBuilder &add(std::type_index componentType);
  void finish();

  template <typename ComponentType>
  ECSEntityBuilder &add(ComponentType component) {
    add(std::make_any<ComponentType>(std::move(component)));

    return *this;
  }

  template <typename ComponentType> ECSEntityBuilder &add() {
    add(std::type_index(typeid(ComponentType)));

    return *this;
  }

  template <typename FieldType, typename ComponentType>
  ECSEntityBuilder &set(FieldType ComponentType::*field, FieldType &&value) {
    const auto it{components.find(std::type_index(typeid(ComponentType)))};

    if (it == components.end()) {
      const ComponentType *seed{
          recipeName.empty()
              ? prefab.getDefault<ComponentType>()
              : prefab.getRecipeDefault<ComponentType>(recipeName)};
      ComponentType component{seed ? *seed : ComponentType{}};
      component.*field = std::forward<FieldType>(value);
      add(std::make_any<ComponentType>(std::move(component)));
    } else {
      auto &comp{std::any_cast<ComponentType &>(it->second)};
      comp.*field = std::forward<FieldType>(value);
    }

    return *this;
  }

private:
  ECSContext &context;
  ECSPrefab &prefab;
  std::string entityId;
  std::string recipeName;
  std::unordered_map<std::type_index, std::any> components;
};
