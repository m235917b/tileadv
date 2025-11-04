#include "asc/asc.hpp"

#include <iostream>

#include <any>

#include "asc/resources.hpp"
#include "asc/systemregistry.hpp"
#include "asc/systems.hpp"
#include "chunk/chunk.hpp"
#include "ecs/ecs.hpp"
#include "ecsgui/ecsgui.hpp"
#include "eventhandler/keydowneventhandler.hpp"
#include "eventhandler/keyupeventhandler.hpp"
#include "eventhandler/moveintenteventhandler.hpp"
#include "player/player.hpp"
#include "player/systems.hpp"
#include "view/systems.hpp"
#include "view/view.hpp"

ASC::ASC()
    : ecsContext(), ecsApi(ecsContext), renderContext(),
      renderContextWrapper(renderContext), guiContext(renderContextWrapper) {};

void ASC::initResources() { initASCResources(ecsContext, ecsApi); }

void ASC::initPhases() {
  ecsContext.getScheduler().addOneShotPhase("spawn", false, true);
  ecsContext.getScheduler().addPhase("input", true, true);
  ecsContext.getScheduler().addPhase("movement", true, false);
  ecsContext.getScheduler().addPhase("logic", true, true);
  ecsContext.getScheduler().addPhase("post_logic", true, true);
  ecsContext.getScheduler().addPhase("rendering_preparation", true, false);
  ecsContext.getScheduler().addPhase("rendering", false, false);
}

void ASC::initSystems() {
  for (auto &&regEntry : getViewSystems()) {
    ecsContext.getScheduler().registerSystem(
        systemIdToPhase.at(regEntry.system_id), std::move(regEntry.system_id),
        std::move(regEntry.system));
  }

  for (auto &&regEntry : getASCSystems()) {
    ecsContext.getScheduler().registerSystem(
        systemIdToPhase.at(regEntry.system_id), std::move(regEntry.system_id),
        std::move(regEntry.system));
  }

  for (auto &&regEntry : getPlayerSystems()) {
    ecsContext.getScheduler().registerSystem(
        systemIdToPhase.at(regEntry.system_id), std::move(regEntry.system_id),
        std::move(regEntry.system));
  }
}

void ASC::initEventListeners() {
  subscribeKeyDownEventHandler(ecsContext);
  subscribeMoveIntentEventHandler(ecsContext);
  subscribeKeyUpEventHandler(ecsContext);
}

void ASC::initCommandHandlers() {}

void ASC::run() {
  initView(ecsContext, renderContext);
  initECSGUI(ecsContext, guiContext);
  initChunk(ecsContext);
  initPlayer(ecsContext, ecsApi);

  initResources();
  initPhases();
  initSystems();
  initEventListeners();
  initCommandHandlers();

  ecsContext.getScheduler().bootstrap();

  ecsContext.getScheduler().updateOneShotPhase("spawn", 0.f);

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

void ASC::destroy() { destroyView(ecsContext, renderContext); }
