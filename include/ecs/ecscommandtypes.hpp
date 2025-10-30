#pragma once

#include <any>
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

struct AddComponent {
  std::string entityId;
  std::type_index componentType;
  std::any payload;
};

template <> struct is_reserved_command<PrefabCommand> : std::true_type {};
template <> struct is_reserved_command<PrintCommand> : std::true_type {};
