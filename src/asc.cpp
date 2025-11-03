#include "asc.hpp"

#include <iostream>

#include <any>

#include "ecs/ecs.hpp"
#include "eventhandler/keydowneventhandler.hpp"
#include "resources/applicationstateresource.hpp"
#include "resources/chunkresource.hpp"
#include "resources/guicontextresource.hpp"
#include "resources/rendercontextresource.hpp"
#include "systems/keyinputsystem.hpp"
#include "systems/rendersystem.hpp"
#include "view/view.hpp"

ECSASC::ECSASC()
    : ecsContext(), ecsApi(ecsContext), renderContext(),
      renderContextWrapper(renderContext), guiContext(renderContextWrapper) {};

void ECSASC::initSDL() {
  initView(renderContext);
  loadTextures(renderContext, ecsContext);
}

void ECSASC::initResources() {
  initRenderContextResource(ecsContext, renderContext);
  initGUIContextResource(ecsContext, guiContext);

  initApplicationStateResource(ecsContext);
  initChunkResource(ecsContext);
}

void ECSASC::initPhases() {
  ecsContext.getScheduler().addPhase("input", true, false);
  ecsContext.getScheduler().addPhase("rendering", true, true);
}

void ECSASC::initSystems() {
  registerRenderSystem("rendering", ecsContext);
  registerKeyInputSystem("input", ecsContext);
}

void ECSASC::initEventListeners() { subscribeKeyDownEventHandler(ecsContext); }

void ECSASC::initCommandHandlers() {}

void ECSASC::run() {
  initSDL();
  initResources();
  initPhases();
  initSystems();
  initEventListeners();
  initCommandHandlers();

  ecsContext.getScheduler().bootstrap();

  bool run{true};
  auto previousTick{SDL_GetTicks()};

  while (run) {
    const auto dt{(SDL_GetTicks() - previousTick) / 1000.f};
    previousTick = SDL_GetTicks();

    ecsContext.getScheduler().update(dt);

    if (ecsContext.getResourceManager()
            .getResource<ApplicationStateResource>()
            ->state == ApplicationState::QUIT) {
      run = false;
    }

    const auto remaining{(1000.f / float(framerate)) -
                         float(SDL_GetTicks() - previousTick)};
    if (remaining > 0.f) {
      SDL_Delay(remaining);
    }
    std::cout << 1000.f / float(SDL_GetTicks() - previousTick) << std::endl;
  }

  destroy();
}

void ECSASC::destroy() {
  destroyTextures(ecsContext);
  destroyView(renderContext);
}
