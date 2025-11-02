#include "systems/rendersystem.hpp"

#include "ecs/ecscontext.hpp"
#include "gui/guicontext.hpp"
#include "view/ecsview.hpp"

std::string registerRenderSystem(const std::string &phase, ECSContext &context,
                                 const RenderContext &renderContext,
                                 const GUIContext &) {
  const std::string id{"render"};
  context.getScheduler().registerSystem(
      phase, id, [&renderContext](ECSContext &, const float) {
        SDL_SetRenderDrawColor(renderContext.renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderContext.renderer);

        SDL_SetRenderDrawColor(renderContext.renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderLine(renderContext.renderer, 0, 0, 500, 500);

        SDL_RenderPresent(renderContext.renderer);
      });
  return id;
}
