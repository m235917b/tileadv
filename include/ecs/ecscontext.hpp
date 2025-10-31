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

  void
  view(const std::vector<std::type_index> &types,
       const std::function<void(const std::string &,
                                const std::vector<const std::any *> &)> &f);

  template <typename... Ts, typename Func> void view(Func &&f) {
    store.view<Ts...>(std::forward<Func>(f));
  }

private:
  ECSStore store;
  ECSScheduler scheduler;
  ECSCommandBuffer commandBuffer;
  ECSEventBus eventBus;

  ECSStore &getStore();

  friend class ECSAPI;
};
