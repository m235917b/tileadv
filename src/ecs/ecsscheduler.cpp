#include <algorithm>

#include "ecs/ecscontext.hpp"
#include "ecs/ecsscheduler.hpp"

ECSScheduler::ECSScheduler(ECSContext &context)
    : context(context), phasesPre(), phasesPost(), systems(), enabled(),
      idToPhase() {}

void ECSScheduler::bootstrap() {
  context.getEventBus().dispatch();
  context.getCommandBuffer().flush();
}

void ECSScheduler::update(const float dt) {
  for (const auto &phase : phasesPre) {
    const auto it{systems.find(phase)};
    if (it != systems.end()) {
      for (const auto &[id, system] : it->second) {
        if (enabled[id]) {
          system(context, dt);
        }
      }
    }
  }

  context.getEventBus().dispatch();

  for (const auto &phase : phasesPost) {
    const auto it{systems.find(phase)};
    if (it != systems.end()) {
      for (const auto &[id, system] : it->second) {
        if (enabled[id]) {
          system(context, dt);
        }
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

void ECSScheduler::registerSystem(
    const std::string &phase, std::string systemId,
    std::function<void(ECSContext &, const float)> system) {
  const auto itPre{std::find_if(
      phasesPre.begin(), phasesPre.end(),
      [&phaseExt = phase](const auto &phase) { return phase == phaseExt; })};

  const auto itPost{std::find_if(
      phasesPost.begin(), phasesPost.end(),
      [&phaseExt = phase](const auto &phase) { return phase == phaseExt; })};

  if (itPre == phasesPre.end() && itPost == phasesPost.end()) {
    return;
  }

  enabled[systemId] = true;
  idToPhase[systemId] = phase;
  systems[phase][std::move(systemId)] = std::move(system);
}

void ECSScheduler::removeSystem(const std::string &systemId) {
  const auto it{idToPhase.find(systemId)};

  if (it == idToPhase.end()) {
    return;
  }

  enabled.erase(enabled.find(systemId), enabled.end());
  idToPhase.erase(idToPhase.find(systemId), idToPhase.end());
  auto &phaseList{systems[systemId]};
  phaseList.erase(phaseList.find(systemId), phaseList.end());

  std::erase_if(systems,
                [](const auto &phaseList) { return phaseList.second.empty(); });
}

void ECSScheduler::enableSystem(const std::string &id) {
  const auto it{enabled.find(id)};

  if (it == enabled.end()) {
    return;
  }

  it->second = true;
}

void ECSScheduler::disableSystem(const std::string &id) {
  const auto it{enabled.find(id)};

  if (it == enabled.end()) {
    return;
  }

  it->second = false;
}
