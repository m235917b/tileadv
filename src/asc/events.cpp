#include "asc/events.hpp"

#include "asc/resources.hpp"
#include "ecs/ecs.hpp"

void subscribeKeyDownEventListener(ECSContext &context) {
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

void subscribeKeyUpEventListener(ECSContext &context) {
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

void initASCEvents(ECSContext &ecsContext) {
  subscribeKeyDownEventListener(ecsContext);
  subscribeKeyUpEventListener(ecsContext);
}
