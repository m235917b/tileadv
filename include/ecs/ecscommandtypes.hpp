#pragma once

#include <any>
#include <set>
#include <string>
#include <type_traits>
#include <typeindex>
#include <utility>

struct PrefabCommand {
  int x;
};

struct PrintCommand {
  std::string text;
};

struct UpsertComponent {
  std::string entityId;
  std::any payload;
};

inline std::set<std::type_index> reserved_commands{
    std::type_index(typeid(PrefabCommand)),
    std::type_index(typeid(PrintCommand)),
    std::type_index(typeid(UpsertComponent))};
