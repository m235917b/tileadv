#include "eventhandler/keydowneventhandler.hpp"

#include <any>

#include <SDL3/SDL.h>

#include "asc/resources.hpp"
#include "ecs/ecs.hpp"
#include "events/keydownevent.hpp"
#include "events/moveIntentEvent.hpp"
#include "player/resources.hpp"

void subscribeKeyDownEventHandler(ECSContext &context) {
  context.getEventBus().subscribe<KeyDownEvent>([](ECSContext &context,
                                                   const KeyDownEvent &event) {
    auto keyMap{*context.getResourceManager().getResource<KeyMapResource>()};
    switch (event.keycode) {
    case SDLK_ESCAPE: {
      context.getCommandBuffer().upsertResource(
          ApplicationStateResource{ApplicationState::QUIT});
      break;
    }
    }

    if (event.keycode == SDLK_W) {
      keyMap.w = true;
    }
    if (event.keycode == SDLK_A) {
      keyMap.a = true;
    }
    if (event.keycode == SDLK_S) {
      keyMap.s = true;
    }
    if (event.keycode == SDLK_D) {
      keyMap.d = true;
    }

    context.getCommandBuffer().upsertResource(keyMap);
  });
}
