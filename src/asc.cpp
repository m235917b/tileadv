#include "asc.hpp"

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
  ecsContext.getScheduler().addPhasePre("input");
  ecsContext.getScheduler().addPhasePost("rendering");
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

  while (run) {
    ecsContext.getScheduler().update(0.0f);

    if (ecsContext.getResourceManager()
            .getResource<ApplicationStateResource>()
            ->state == ApplicationState::QUIT) {
      run = false;
    }
  }

  destroy();
}

void ECSASC::destroy() {
  destroyTextures(ecsContext);
  destroyView(renderContext);
}
