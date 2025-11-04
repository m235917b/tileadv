#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "ecs/ecs.hpp"

class ECSContext;

struct SystemRegEntry {
  std::string system_id;
  SystemFn system;
};

const std::unordered_map<std::string, std::string> systemIdToPhase{
    std::make_pair<std::string, std::string>("render", "rendering"),
    std::make_pair<std::string, std::string>("key_input", "input"),
    std::make_pair<std::string, std::string>("player_movement", "movement"),
    std::make_pair<std::string, std::string>("player_spawn", "spawn"),
    std::make_pair<std::string, std::string>("dummy_spawn", "spawn")};
