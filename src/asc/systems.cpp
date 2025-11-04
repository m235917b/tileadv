#include "asc/systems.hpp"

#include <SDL3/SDL.h>

#include "asc/events.hpp"
#include "ecs/ecscontext.hpp"

const auto keyInputSystem{[](ECSContext &context, const float) {
  SDL_Event e;
  SDL_zero(e);
  while (SDL_PollEvent(&e) == true) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
      context.getEventBus().publish<KeyDownEvent>({e.key.key});
    }
    if (e.type == SDL_EVENT_KEY_UP) {
      context.getEventBus().publish<KeyUpEvent>({e.key.key});
    }
  }
}};

std::vector<SystemRegEntry> getASCSystems() {
  return {SystemRegEntry{"key_input", keyInputSystem}};
}
