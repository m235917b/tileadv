#pragma once

#include "ecs/ecscommandbuffer.hpp"
#include "ecs/ecseventbus.hpp"
#include "ecs/ecsscheduler.hpp"
#include "ecs/ecsstore.hpp"

class ECSContext {
public:
  ECSContext();
  ~ECSContext() = default;

  ECSScheduler &getScheduler();
  ECSCommandBuffer &getCommandBuffer();
  ECSEventBus &getEventBus();
  const ECSStore &getStore() const noexcept;

  void destroyEntity(const std::string &entityId);

  template <typename T>
  void addComponent(const std::string &entityId, T component) {
    store.addComponent(entityId, std::move(component));
  }

  template <typename... Ts, typename Func> void view(Func &&f) {
    store.view<Ts...>(std::forward<Func>(f));
  }

private:
  ECSStore store;
  ECSScheduler scheduler;
  ECSCommandBuffer commandBuffer;
  ECSEventBus eventBus;
};
