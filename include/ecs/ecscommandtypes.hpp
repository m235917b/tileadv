#pragma once

#include <any>
#include <set>
#include <string>
#include <type_traits>
#include <typeindex>
#include <utility>

template <class T> struct is_reserved_command : std::false_type {};

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

template <> struct is_reserved_command<PrefabCommand> : std::true_type {};
template <> struct is_reserved_command<PrintCommand> : std::true_type {};
template <> struct is_reserved_command<UpsertComponent> : std::true_type {};

inline std::set<std::type_index> reserved_commands{
    std::type_index(typeid(PrefabCommand)),
    std::type_index(typeid(PrintCommand)),
    std::type_index(typeid(UpsertComponent))};
