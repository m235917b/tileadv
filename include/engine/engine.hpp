#pragma once

#include <functional>
#include <string>
#include <any>

class GameAPI;

class ECSContext;

using EntityEffectFn = std::function<void(GameAPI &, const std::any &)>;

struct EntityEffect {
  std::string id;
  EntityEffectFn apply;
};

void initEngine(ECSContext &ecsContext);
