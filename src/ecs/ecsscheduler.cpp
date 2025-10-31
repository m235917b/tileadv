#include <algorithm>

#include "ecs/ecscontext.hpp"
#include "ecs/ecsscheduler.hpp"

ECSScheduler::ECSScheduler(ECSContext &context)
    : context(context), phasesPre(), phasesPost(), systems() {}

void ECSScheduler::bootstrap() {
  context.getEventBus().dispatch();
  context.getCommandBuffer().flush();
}

void ECSScheduler::update(const float dt) {
  for (const auto &phase : phasesPre) {
    const auto it{systems.find(phase)};
    if (it != systems.end()) {
      for (const auto &system : it->second) {
        system.second(context, dt);
      }
    }
  }

  context.getEventBus().dispatch();

  for (const auto &phase : phasesPost) {
    const auto it{systems.find(phase)};
    if (it != systems.end()) {
      for (const auto &system : it->second) {
        system.second(context, dt);
      }
    }
  }

  context.getCommandBuffer().flush();
}

void ECSScheduler::addPhasePre(std::string phase) {
  phasesPre.push_back(std::move(phase));
}

void ECSScheduler::addPhasePost(std::string phase) {
  phasesPost.push_back(std::move(phase));
}

void ECSScheduler::removePhase(const std::string &phase) {
  phasesPre.erase(std::remove(phasesPre.begin(), phasesPre.end(), phase),
                  phasesPre.end());
  phasesPost.erase(std::remove(phasesPost.begin(), phasesPost.end(), phase),
                   phasesPost.end());
}

void ECSScheduler::addSystem(
    const std::string &phase, std::string systemId,
    std::function<void(ECSContext &, const float)> system) {
  systems[phase].emplace_back(std::move(systemId), std::move(system));
}

void ECSScheduler::removeSystem(const std::string &phase,
                                const std::string &systemId) {
  const auto it{systems.find(phase)};

  if (it != systems.end()) {
    auto &sysVec{it->second};
    sysVec.erase(
        std::remove_if(sysVec.begin(), sysVec.end(),
                       [&](const auto &p) { return p.first == systemId; }),
        sysVec.end());
  }
}
