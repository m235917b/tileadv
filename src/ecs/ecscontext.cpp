#include <iostream>
#include <typeindex>

#include "ecs/ecscontext.hpp"

ECSContext::ECSContext()
    : store(), resourceManager(), scheduler(*this), commandBuffer(*this),
      eventBus(*this) {
  commandBuffer.registerHandlerInternal<PrintCommand>(
      [](ECSContext &, PrintCommand command) {
        std::cout << command.text << std::endl;
      });

  commandBuffer.registerHandlerInternal<UpsertComponent>(
      [this](ECSContext &, UpsertComponent command) {
        this->store.upsertComponent(command.entityId,
                                    std::move(command.payload));
      });

  commandBuffer.registerHandlerInternal<UpsertResource>(
      [this](ECSContext &, UpsertResource command) {
        this->resourceManager.setResource(
            std::type_index(command.payload.type()),
            std::move(command.payload));
      });
}

ECSScheduler &ECSContext::getScheduler() { return scheduler; }

ECSCommandBuffer &ECSContext::getCommandBuffer() { return commandBuffer; }

ECSEventBus &ECSContext::getEventBus() { return eventBus; }

const ECSStore &ECSContext::getStore() const noexcept { return store; }

const ECSResourceManager &ECSContext::getResourceManager() const noexcept {
  return resourceManager;
}
