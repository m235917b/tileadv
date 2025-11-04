#include "eventhandler/keydowneventhandler.hpp"

#include <any>

#include <SDL3/SDL.h>

#include "ecs/ecs.hpp"
#include "events/keyupevent.hpp"
#include "events/moveIntentEvent.hpp"
#include "resources/applicationstateresource.hpp"
#include "resources/keymapresource.hpp"
#include "resources/playeridresource.hpp"

void subscribeKeyUpEventHandler(ECSContext &context) {
  context.getEventBus().subscribe<KeyUpEvent>([](ECSContext &context,
                                                 const KeyUpEvent &event) {
    auto keyMap{*context.getResourceManager().getResource<KeyMapResource>()};

    if (event.keycode == SDLK_W) {
      keyMap.w = false;
    }
    if (event.keycode == SDLK_A) {
      keyMap.a = false;
    }
    if (event.keycode == SDLK_S) {
      keyMap.s = false;
    }
    if (event.keycode == SDLK_D) {
      keyMap.d = false;
    }

    context.getCommandBuffer().upsertResource(keyMap);
  });
}
