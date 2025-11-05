#pragma once

#include <string>
#include <unordered_map>

#include "engine.hpp"

class ECSContext;

struct EntityEffectTableResource {
  std::unordered_map<std::string, EntityEffect> table;
};

void initEngineResources(ECSContext &ecsContext);
