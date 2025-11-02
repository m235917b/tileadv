#include "eventhandler/keydowneventhandler.hpp"

#include <any>

#include <SDL3/SDL.h>

#include "ecs/ecs.hpp"
#include "events/keydownevent.hpp"
#include "resources/applicationstateresource.hpp"

void subscribeKeyDownEventHandler(ECSContext &context) {
  context.getEventBus().subscribe<KeyDownEvent>(
      [](ECSContext &context, const KeyDownEvent &event) {
        switch (event.keycode) {
        case SDLK_ESCAPE:
          const auto appState{std::make_any<ApplicationStateResource>(
              ApplicationStateResource{ApplicationState::QUIT})};
          const auto res{UpsertResource{appState}};
          context.getCommandBuffer().enqueue<UpsertResource>(res);
          break;
        }
      });
}
