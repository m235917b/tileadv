#include "ecsasc.hpp"

#include <any>

#include "ecs/ecs.hpp"
#include "eventhandler/keydowneventhandler.hpp"
#include "resources/applicationstateresource.hpp"
#include "systems/keyinputsystem.hpp"
#include "systems/rendersystem.hpp"
#include "view/ecsview.hpp"

ECSASC::ECSASC()
    : ecsContext(), ecsApi(ecsContext), renderContext(),
      renderContextWrapper(renderContext), guiContext(renderContextWrapper) {};

void ECSASC::init() {
  initView(renderContext);

  const auto appState{std::make_any<ApplicationStateResource>(
      ApplicationStateResource{ApplicationState::RUNNING})};
  const auto resAny{UpsertResource{appState}};
  ecsContext.getCommandBuffer().enqueue<UpsertResource>(resAny);

  ecsContext.getScheduler().addPhasePre("input");
  ecsContext.getScheduler().addPhasePost("rendering");

  registerRenderSystem("rendering", ecsContext, renderContext, guiContext);
  registerKeyInputSystem("input", ecsContext);

  subscribeKeyDownEventHandler(ecsContext);

  ecsContext.getScheduler().bootstrap();
}

void ECSASC::run() {
  init();

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

void ECSASC::destroy() { destroyView(renderContext); }
