#include "asc/asc.hpp"

#include <iostream>

#include <any>

#include "actor/actor.hpp"
#include "asc/events.hpp"
#include "asc/resources.hpp"
#include "asc/systemregistry.hpp"
#include "asc/systems.hpp"
#include "chunk/chunk.hpp"
#include "ecs/ecs.hpp"
#include "ecsgui/ecsgui.hpp"
#include "engine/engine.hpp"
#include "npc/npc.hpp"
#include "npc/systems.hpp"
#include "player/player.hpp"
#include "player/systems.hpp"
#include "view/systems.hpp"
#include "view/view.hpp"

ASC::ASC()
    : ecsContext(), ecsApi(ecsContext), renderContext(),
      renderContextWrapper(renderContext), guiContext(renderContextWrapper) {
  initPhases();

  initASC();
  initView(ecsContext, renderContext);
  initECSGUI(ecsContext, guiContext);
  initEngine(ecsContext);
  initChunk(ecsContext);
  initActors(ecsContext);
  initPlayer(ecsContext, ecsApi);
  initNPCs(ecsApi);

  initSystems();

  ecsContext.getScheduler().bootstrap();
};

ASC::~ASC() { destroy(); }

void ASC::initASC() {
  initASCResources(ecsContext, ecsApi);
  initASCEvents(ecsContext);
}

void ASC::initPhases() {
  ecsContext.getScheduler().addOneShotPhase("spawn", false, true);
  ecsContext.getScheduler().addPhase("input", true, true);
  ecsContext.getScheduler().addPhase("movement", true, false);
  ecsContext.getScheduler().addPhase("logic", true, true);
  ecsContext.getScheduler().addPhase("post_logic", true, true);
  ecsContext.getScheduler().addPhase("rendering_preparation", true, false);
  ecsContext.getScheduler().addPhase("rendering", false, false);
}

void ASC::initModuleSystems(const std::vector<SystemRegEntry> &systems) {
  for (auto &&regEntry : systems) {
    ecsContext.getScheduler().registerSystem(
        systemIdToPhase.at(regEntry.system_id), std::move(regEntry.system_id),
        std::move(regEntry.system));
  }
}

void ASC::initSystems() {
  initModuleSystems(getASCSystems());
  initModuleSystems(getViewSystems());
  initModuleSystems(getPlayerSystems());
  initModuleSystems(getNPCSystems());
}

void ASC::run() {
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
    // std::cout << 1000.f / float(SDL_GetTicks() - previousTick) << std::endl;
  }
}

void ASC::destroy() { destroyView(ecsContext, renderContext); }

ECSContext &ASC::getECSContext() { return ecsContext; }
