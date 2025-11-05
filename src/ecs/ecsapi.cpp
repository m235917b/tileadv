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

void ECSAPI::print(std::string text) {
  context.getCommandBuffer().print(std::move(text));
}
