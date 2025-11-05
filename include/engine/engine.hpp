#pragma once

#include <functional>
#include <string>

#include "ecs/ecsapi.hpp"

class ECSContext;

using EntityEffectFn = std::function<void(ECSAPI &, const std::any &)>;

struct EntityEffect {
  std::string id;
  EntityEffectFn apply;
};

void initEngine(ECSContext &ecsContext);
