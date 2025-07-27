#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "gui/guicomponent.hpp"

template <typename Func, typename... T>
concept TraversalCallable = requires(Func f, GUIComponent &comp, T... args) {
  { std::invoke(f, comp, args...) } -> std::convertible_to<bool>;
};

class GUITreeWalker {
public:
  template <typename Func, typename... T>
    requires TraversalCallable<Func, T...>
  static void traverse(GUIComponent &component, Func &&action, T... args) {
    auto recurse{std::invoke(std::forward<Func>(action), component, args...)};

    if (recurse) {
      component.forEachChild([&action, &args...](GUIComponent &child) {
        traverse(child, std::forward<Func>(action), args...);
      });
    }
  }

  template <typename Func, typename... T>
    requires TraversalCallable<Func, T...>
  static void traverse(GUIComponent &component, Func &&action, T... args,
                       bool &stop) {
    if (stop) {
      return;
    }

    auto recurse{std::invoke(std::forward<Func>(action), component, args...)};

    if (recurse) {
      component.forEachChild([&action, &args...](GUIComponent &child) {
        traverse(child, std::forward<Func>(action), args...);
      });
    }
  }

  static void traverse(GUIComponent &component,
                       std::function<bool(GUIComponent &component)> action);

  static void traverse(GUIComponent &component,
                       std::function<bool(GUIComponent &component)> action,
                       bool &stop);
};
