#pragma once

#include "ecs/ecscommandbuffer.hpp"
#include "ecs/ecseventbus.hpp"
#include "ecs/ecsresourcemanager.hpp"
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
  const ECSResourceManager &getResourceManager() const noexcept;

private:
  ECSStore store;
  ECSResourceManager resourceManager;
  ECSScheduler scheduler;
  ECSCommandBuffer commandBuffer;
  ECSEventBus eventBus;
};
