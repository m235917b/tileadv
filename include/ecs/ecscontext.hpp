#pragma once

#include "ecs/ecsscheduler.hpp"
#include "ecs/ecsstore.hpp"


class ECSContext {
public:
  ECSContext();
  ~ECSContext() = default;

  ECSStore &getStore();
  ECSScheduler &getScheduler();

private:
  ECSStore store;
  ECSScheduler scheduler;
};
