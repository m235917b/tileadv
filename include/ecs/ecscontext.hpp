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

  template <typename... Ts, typename Func> void view(Func &&f) {
    store.view<Ts...>(std::forward<Func>(f));
  }

  void
  view(std::vector<std::type_index> types,
       std::function<void(const std::string &, const std::vector<std::any *> &)>
           f) {
    store.view(std::move(types), std::move(f));
  }

private:
  ECSStore store;
  ECSScheduler scheduler;
  ECSCommandBuffer commandBuffer;
  ECSEventBus eventBus;
};
