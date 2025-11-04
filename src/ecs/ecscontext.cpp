#include <iostream>
#include <string>
#include <typeindex>

#include "ecs/ecscontext.hpp"

ECSContext::ECSContext()
    : store(), resourceManager(), scheduler(*this), commandBuffer(*this),
      eventBus(*this) {}

ECSScheduler &ECSContext::getScheduler() { return scheduler; }

ECSCommandBuffer &ECSContext::getCommandBuffer() { return commandBuffer; }

ECSEventBus &ECSContext::getEventBus() { return eventBus; }

const ECSStore &ECSContext::getStore() const noexcept { return store; }

const ECSResourceManager &ECSContext::getResourceManager() const noexcept {
  return resourceManager;
}
