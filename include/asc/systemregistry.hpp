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
    std::make_pair<std::string, std::string>("render_clear",
                                             "render_preparation"),
    std::make_pair<std::string, std::string>("render", "rendering"),
    std::make_pair<std::string, std::string>("gui", "gui_rendering"),
    std::make_pair<std::string, std::string>("render_present",
                                             "post_rendering"),
    std::make_pair<std::string, std::string>("sdl_input", "input"),
    std::make_pair<std::string, std::string>("player_movement", "movement"),
    std::make_pair<std::string, std::string>("player_spawn", "spawn"),
    std::make_pair<std::string, std::string>("dummy_spawn", "spawn"),
    std::make_pair<std::string, std::string>("collision", "post_logic"),
    std::make_pair<std::string, std::string>("garbage_collector",
                                             "post_logic")};
