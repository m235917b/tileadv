#include <iostream>

#include "ecs/ecscontext.hpp"

ECSContext::ECSContext()
    : store(), scheduler(*this), commandBuffer(*this), eventBus(*this) {
  commandBuffer.registerHandlerInternal<PrintCommand>(
      []([[maybe_unused]] ECSContext &context, const PrintCommand &command) {
        std::cout << command.text << std::endl;
      });

  commandBuffer.registerHandlerInternal<PrefabCommand>(
      []([[maybe_unused]] ECSContext &context,
         [[maybe_unused]] const PrefabCommand &command) {});
}

ECSScheduler &ECSContext::getScheduler() { return scheduler; }

ECSCommandBuffer &ECSContext::getCommandBuffer() { return commandBuffer; }

ECSEventBus &ECSContext::getEventBus() { return eventBus; }

const ECSStore &ECSContext::getStore() const noexcept { return store; }

void ECSContext::destroyEntity(const std::string &entityId) {
  store.destroyEntity(entityId);
}
