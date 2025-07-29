#pragma once

#include <concepts>
#include <vector>

#include <SDL3/SDL.h>

#include "gui/guicomponent.hpp"

struct GUILayoutData {
  GUIComponent *component;
  SDL_FRect layout;
};

class GUILayoutBuffer {
public:
  GUILayoutBuffer();

  void allocate(const size_t size);

  bool push(GUILayoutData data);

  template <typename Func>
    requires std::invocable<Func, GUILayoutData &>
  void forEach(Func &&action, bool reverse = false) {
    if (!reverse) {
      for (auto i{0}; i < head; ++i) {
        std::invoke(std::forward<Func>(action), buffer[i]);
      }
    } else {
      for (auto i{head - 1}; i >= 0; --i) {
        std::invoke(std::forward<Func>(action), buffer[i]);
      }
    }
  }

  template <typename Func>
    requires std::invocable<Func, GUILayoutData &>
  void forEach(Func &&action, bool &stop, bool reverse = false) {
    if (!reverse) {
      for (auto i{0}; i < head; ++i) {
        if (stop) {
          return;
        }

        std::invoke(std::forward<Func>(action), buffer[i]);
      }
    } else {
      for (auto i{head - 1}; i >= 0; --i) {
        if (stop) {
          return;
        }

        std::invoke(std::forward<Func>(action), buffer[i]);
      }
    }
  }

private:
  int head;
  std::vector<GUILayoutData> buffer;
};
