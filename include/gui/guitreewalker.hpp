#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <utility>
#include <vector>


#include "gui/guicomponent.hpp"

template <typename Func, typename... T>
concept TraversalCallableParametrized =
    requires(Func f, GUIComponent &comp, T... args) {
      { std::invoke(f, comp, args...) } -> std::convertible_to<bool>;
    };

class GUITreeWalker {
public:
  template <typename Func, typename... T>
    requires TraversalCallableParametrized<Func, T...>
  static void traverse(GUIComponent &component, Func &&action, T... args) {
    auto recurse{std::invoke(std::forward<Func>(action), component, args...)};

    if (recurse) {
      component.forEachChild([&action, &args...](GUIComponent &child) {
        traverse(child, std::forward<Func>(action), args...);
      });
    }
  }

  template <typename Func, typename... T>
    requires TraversalCallableParametrized<Func, T...>
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

  template <typename Func>
    requires std::invocable<Func, GUIComponent &>
  static void traverse(GUIComponent &component, Func &&action) {
    const auto recurse{std::invoke(std::forward<Func>(action), component)};

    if (recurse) {
      component.forEachChild([&action](GUIComponent &child) {
        GUITreeWalker::traverse(child, std::forward<Func>(action));
      });
    }
  }

  template <typename Func>
    requires std::invocable<Func, GUIComponent &>
  static void traverse(GUIComponent &component, Func &&action, bool &stop) {
    if (stop) {
      return;
    }

    const auto recurse{std::invoke(std::forward<Func>(action), component)};

    if (recurse) {
      component.forEachChild([&action, &stop](GUIComponent &child) {
        GUITreeWalker::traverse(child, std::forward<Func>(action), stop);
      });
    }
  }
};
