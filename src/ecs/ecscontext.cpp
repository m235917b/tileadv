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

void ECSContext::addViewSystem(
    const std::string &phase, std::string systemId,
    std::vector<std::type_index> types,
    std::function<void(ECSContext &, const float, const std::string &,
                       const std::vector<const std::any *> &)>
        system) {
  const auto sysFunc{
      [this, types = std::move(types),
       system = std::move(system)](ECSContext &context, const float dt) {
        const auto sysFuncView{
            [&](const std::string &entityId,
                const std::vector<const std::any *> &components) {
              system(context, dt, entityId, components);
            }};
        store.view(types, sysFuncView);
      }};

  scheduler.registerSystem(phase, systemId, sysFunc);
}

void ECSContext::subscribeViewEventListener(
    const std::type_index &type, std::vector<std::type_index> types,
    std::function<void(ECSContext &, const std::any &, const std::string &,
                       const std::vector<const std::any *> &)>
        listener) {
  const auto listenerFunc{
      [this, types = std::move(types), listener = std::move(listener)](
          ECSContext &context, const std::any &event) {
        const auto listenerView{
            [&](const std::string &entityId,
                const std::vector<const std::any *> &components) {
              listener(context, event, entityId, components);
            }};
        store.view(types, listenerView);
      }};

  eventBus.subscribe(type, listenerFunc);
}
