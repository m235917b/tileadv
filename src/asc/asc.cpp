#include "asc/asc.hpp"

#include <iostream>

#include <any>
#include <typeindex>

#include "actor/actor.hpp"
#include "actor/systems.hpp"
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

// TODO: Chunk loading on edge
// TODO: Make internals any based?

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
  initNPCs(ecsContext, ecsApi);

  initSystems();

  ecsContext.getScheduler().bootstrap();

  guiContext.init();
};

ASC::~ASC() { destroy(); }

void ASC::initASC() {
  initASCResources(ecsContext, ecsApi, guiContext);
  initASCEvents(ecsContext);
}

void ASC::initPhases() {
  ecsContext.getScheduler().addOneShotPhase("spawn", false, true);
  ecsContext.getScheduler().addPhase("input", true, true);
  ecsContext.getScheduler().addPhase("movement", true, false);
  ecsContext.getScheduler().addPhase("logic", true, true);
  ecsContext.getScheduler().addPhase("post_logic", true, true);
  ecsContext.getScheduler().addPhase("render_preparation", false, false);
  ecsContext.getScheduler().addPhase("rendering", false, false);
  ecsContext.getScheduler().addPhase("gui_rendering", false, false);
  ecsContext.getScheduler().addPhase("post_rendering", false, false);
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
  initModuleSystems(getActorSystems());
}

void ASC::run() {
  ecsContext.getScheduler().updateOneShotPhase("spawn", 0.f);

  auto previousTick{SDL_GetTicks()};

  const auto appState{std::any_cast<ApplicationStateResource>(
      ecsContext.getResourceManager().getResource(
          std::type_index(typeid(ApplicationStateResource))))};

  auto previousAppState{appState->state};

  while (appState->state != ApplicationState::QUIT) {
    const auto dt{(SDL_GetTicks() - previousTick) / 1000.f};
    previousTick = SDL_GetTicks();
    if (previousAppState != appState->state) {
      switch (appState->state) {
      case ApplicationState::MAIN_MENU:
        ecsContext.getScheduler().disablePhase("movement");
        ecsContext.getScheduler().disablePhase("logic");
        ecsContext.getScheduler().disablePhase("post_logic");
        break;
      case ApplicationState::RUNNING:
        ecsContext.getScheduler().enablePhase("movement");
        ecsContext.getScheduler().enablePhase("logic");
        ecsContext.getScheduler().enablePhase("post_logic");
        break;
      case ApplicationState::QUIT:
        break;
      }
    }

    ecsContext.getScheduler().update(dt);

    previousAppState = appState->state;

    const auto remaining{(1000.f / float(framerate)) -
                         float(SDL_GetTicks() - previousTick)};
    if (remaining > 0.f) {
      SDL_Delay(remaining);
    }
    std::cout << 1000.f / float(SDL_GetTicks() - previousTick) << std::endl;
  }
}

void ASC::destroy() { destroyView(ecsContext, renderContext); }

ECSContext &ASC::getECSContext() { return ecsContext; }

ECSAPI &ASC::getECSAPI() { return ecsApi; }

GUIContext &ASC::getGUIContext() { return guiContext; }
