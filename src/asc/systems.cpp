#include "asc/systems.hpp"

#include <SDL3/SDL.h>

#include "asc/events.hpp"
#include "asc/resources.hpp"
#include "ecs/ecscontext.hpp"
#include "view/resources.hpp"
#include "view/view.hpp"

const auto sdlInputSystem{[](ECSContext &context, const float) {
  const auto cam{
      std::any_cast<CameraResource>(context.getResourceManager().getResource(
          std::type_index(typeid(CameraResource))))};
  context.getCommandBuffer().patchResource(
      std::type_index(typeid(MousePosResource)),
      [cameraX = cam->posX, cameraY = cam->posY](std::any &res) {
        auto &pos{std::any_cast<MousePosResource &>(res)};
        SDL_GetMouseState(&pos.x, &pos.y);
        const auto &tiles{getTileFromPixel(pos.x, pos.y, cameraX, cameraY)};
        pos.tileX = tiles.first;
        pos.tileY = tiles.second;
      });

  SDL_Event e;
  SDL_zero(e);
  while (SDL_PollEvent(&e) == true) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
      context.getEventBus().publish<KeyDownEvent>({e.key.key});
    }
    if (e.type == SDL_EVENT_KEY_UP) {
      context.getEventBus().publish<KeyUpEvent>({e.key.key});
    }
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
      context.getEventBus().publish<MouseDownEvent>({e.button.button});
    }
  }
}};

std::vector<SystemRegEntry> getASCSystems() {
  return {SystemRegEntry{"sdl_input", sdlInputSystem}};
}
