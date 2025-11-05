#pragma once

#include <any>
#include <functional>
#include <string>
#include <typeindex>
#include <vector>

#include "ecs/ecscontext.hpp"
#include "ecs/ecsentitybuilder.hpp"
#include "ecs/ecsprefab.hpp"

class ECSAPI {
public:
  ECSAPI(ECSContext &context);
  ~ECSAPI() = default;

  ECSPrefab &getPrefab();

  ECSEntityBuilder createEntity(std::string entityId);
  ECSEntityBuilder instantiateEntity(std::string entityId,
                                     std::string recipeId);
  void print(std::string text);

  template <typename T>
  void upsertComponent(std::string entityId, T component) {
    context.getCommandBuffer().upsertComponent<T>(std::move(entityId),
                                                  std::move(component));
  }

  template <typename T>
  const T *getComponent(const std::string &entityId) const {
    return context.getStore().getComponent<T>(entityId);
  }

  template <typename ComponentType, typename FieldType>
  void patchComponent(std::string entityId, FieldType ComponentType::*field,
                      FieldType val) {
    context.getCommandBuffer().patchComponent<ComponentType, FieldType>(
        std::move(entityId), field, std::move(val));
  }

  template <typename ComponentType, typename FieldType>
  void patchResource(FieldType ComponentType::*field, FieldType val) {
    context.getCommandBuffer().patchResource<ComponentType, FieldType>(
        field, std::move(val));
  }

private:
  ECSContext &context;
  ECSPrefab prefab;
};
