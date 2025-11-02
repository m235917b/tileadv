#include "systems/keyinputsystem.hpp"

#include <SDL3/SDL.h>

#include "ecs/ecscontext.hpp"
#include "events/keydownevent.hpp"

std::string registerKeyInputSystem(const std::string &phase,
                                   ECSContext &ecsContext) {
  std::string id{"key_input"};

  ecsContext.getScheduler().registerSystem(
      phase, id, [](ECSContext &context, const float) {
        SDL_Event e;
        SDL_zero(e);
        while (SDL_PollEvent(&e) == true) {
          switch (e.type) {
          case SDL_EVENT_KEY_DOWN:
            context.getEventBus().publish<KeyDownEvent>({e.key.key});
            break;
          }
        }
      });

  return id;
}
