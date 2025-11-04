#include "asc.hpp"

#include <iostream>

#include <any>

#include "ecs/ecs.hpp"
#include "eventhandler/keydowneventhandler.hpp"
#include "eventhandler/keyupeventhandler.hpp"
#include "eventhandler/moveintenteventhandler.hpp"
#include "prefabs/playerprefab.hpp"
#include "resources/applicationstateresource.hpp"
#include "resources/chunkresource.hpp"
#include "resources/guicontextresource.hpp"
#include "resources/keymapresource.hpp"
#include "resources/playeridresource.hpp"
#include "resources/rendercontextresource.hpp"
#include "systems/keyinputsystem.hpp"
#include "systems/movementsystem.hpp"
#include "systems/rendersystem.hpp"
#include "systems/spawnplayersystem.hpp"
#include "view/view.hpp"

ECSASC::ECSASC()
    : ecsContext(), ecsApi(ecsContext), renderContext(),
      renderContextWrapper(renderContext), guiContext(renderContextWrapper) {};

void ECSASC::initSDL() { initView(renderContext, ecsContext); }

void ECSASC::initPrefabs() { registerPlayerPrefab(ecsApi); }

void ECSASC::initResources() {
  initRenderContextResource(ecsContext, renderContext);
  initGUIContextResource(ecsContext, guiContext);
  initKeyMapResource(ecsContext);

  initApplicationStateResource(ecsContext);
  initChunkResource(ecsContext);
  initPlayerIDResource(ecsContext);
}

void ECSASC::initPhases() {
  ecsContext.getScheduler().addOneShotPhase("spawnPlayer", false, true);
  ecsContext.getScheduler().addPhase("input", true, true);
  ecsContext.getScheduler().addPhase("movement", true, false);
  ecsContext.getScheduler().addPhase("logic", true, true);
  ecsContext.getScheduler().addPhase("post_logic", true, true);
  ecsContext.getScheduler().addPhase("rendering_preparation", true, false);
  ecsContext.getScheduler().addPhase("rendering", false, false);
}

void ECSASC::initSystems() {
  registerRenderSystem("rendering", ecsContext);
  registerKeyInputSystem("input", ecsContext);
  registerSpawnPlayerSystem("spawnPlayer", ecsContext, ecsApi);
  registerMovementSystem("movement", ecsContext);
}

void ECSASC::initEventListeners() {
  subscribeKeyDownEventHandler(ecsContext);
  subscribeMoveIntentEventHandler(ecsContext);
  subscribeKeyUpEventHandler(ecsContext);
}

void ECSASC::initCommandHandlers() {}

void ECSASC::run() {
  initSDL();
  initPrefabs();
  initResources();
  initPhases();
  initSystems();
  initEventListeners();
  initCommandHandlers();

  ecsContext.getScheduler().bootstrap();

  ecsContext.getScheduler().updateOneShotPhase("spawnPlayer", 0.f);

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

void ECSASC::destroy() { destroyView(renderContext, ecsContext); }
