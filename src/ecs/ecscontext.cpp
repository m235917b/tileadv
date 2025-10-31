#include <iostream>

#include "ecs/ecscontext.hpp"

ECSContext::ECSContext()
    : store(), scheduler(*this), commandBuffer(*this), eventBus(*this) {
  commandBuffer.registerHandlerInternal<PrintCommand>(
      []([[maybe_unused]] ECSContext &context, PrintCommand command) {
        std::cout << command.text << std::endl;
      });

  commandBuffer.registerHandlerInternal<PrefabCommand>(
      []([[maybe_unused]] ECSContext &context,
         [[maybe_unused]] PrefabCommand command) {});

  commandBuffer.registerHandlerInternal<UpsertComponent>(
      [this]([[maybe_unused]] ECSContext &context, UpsertComponent command) {
        this->store.upsertComponent(command.entityId,
                                    std::move(command.payload));
      });
}

ECSScheduler &ECSContext::getScheduler() { return scheduler; }

ECSCommandBuffer &ECSContext::getCommandBuffer() { return commandBuffer; }

ECSEventBus &ECSContext::getEventBus() { return eventBus; }

const ECSStore &ECSContext::getStore() const noexcept { return store; }

ECSStore &ECSContext::getStore() { return store; }

void ECSContext::view(
    const std::vector<std::type_index> &types,
    const std::function<void(const std::string &,
                             const std::vector<const std::any *> &)> &f) {
  store.view(std::move(types), std::move(f));
};
