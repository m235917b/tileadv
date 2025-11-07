#include "asc/systems.hpp"

#include <any>
#include <functional>
#include <typeindex>

#include <SDL3/SDL.h>

#include "asc/events.hpp"
#include "asc/resources.hpp"
#include "ecs/ecscontext.hpp"
#include "gui/guicontext.hpp"
#include "view/resources.hpp"
#include "view/view.hpp"

const auto sdlInputSystem{[](ECSContext &context, const float) {
  const auto cam{
      std::any_cast<CameraResource>(context.getResourceManager().getResource(
          std::type_index(typeid(CameraResource))))};
  const auto appState{std::any_cast<ApplicationStateResource>(
      context.getResourceManager().getResource(
          std::type_index(typeid(ApplicationStateResource))))};

  SDL_Event e;
  SDL_zero(e);
  while (SDL_PollEvent(&e) == true) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
      context.getEventBus().publish(
          std::make_any<KeyDownEvent>(KeyDownEvent{e.key.key}));
      if (appState->state == ApplicationState::RUNNING) {
        context.getEventBus().publish(
            std::make_any<GameKeyDownEvent>(GameKeyDownEvent{e.key.key}));
      }
    }
    if (e.type == SDL_EVENT_KEY_UP) {
      if (appState->state == ApplicationState::RUNNING) {
        context.getEventBus().publish(
            std::make_any<GameKeyUpEvent>(GameKeyUpEvent{e.key.key}));
      }
    }
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
      context.getEventBus().publish(
          std::make_any<MouseDownEvent>(MouseDownEvent{e.button.button}));
      if (appState->state == ApplicationState::RUNNING) {
        context.getEventBus().publish(std::make_any<GameMouseDownEvent>(
            GameMouseDownEvent{e.button.button}));
      }
    }
    if (e.type == SDL_EVENT_MOUSE_MOTION) {
      float mouseX;
      float mouseY;
      SDL_GetMouseState(&mouseX, &mouseY);
      const auto &tiles{getTileFromPixel(mouseX, mouseY, cam->posX, cam->posY)};
      context.getCommandBuffer().patchResource(
          std::type_index(typeid(MousePosResource)), [=](std::any &res) {
            auto &pos{std::any_cast<MousePosResource &>(res)};
            pos.x = mouseX;
            pos.y = mouseY;
            pos.tileX = tiles.first;
            pos.tileY = tiles.second;
          });
      context.getEventBus().publish(std::make_any<MouseMotionEvent>(
          MouseMotionEvent{mouseX, mouseY, tiles.first, tiles.second}));
    }
  }
}};

const auto guiSystem{[](ECSContext &context, const float) {
  const auto guiContext{std::any_cast<GUIContextResource>(
                            context.getResourceManager().getResource(
                                std::type_index(typeid(GUIContextResource))))
                            ->guiContext};
  guiContext->update();
}};

std::vector<SystemRegEntry> getASCSystems() {
  return {SystemRegEntry{"sdl_input", sdlInputSystem},
          SystemRegEntry{"gui", guiSystem}};
}
