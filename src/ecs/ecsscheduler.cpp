#include <algorithm>

#include "ecs/ecscontext.hpp"
#include "ecs/ecsscheduler.hpp"

ECSScheduler::ECSScheduler(ECSContext &context)
    : context(context), phases(), lookUpByEntity(), lookUpByPhase() {}

void ECSScheduler::bootstrap() {
  context.getEventBus().dispatch();
  context.getCommandBuffer().flush();
}

void ECSScheduler::update(const float dt) {
  for (const auto &phase : phases) {
    if (phase.enabled) {
      for (const auto &system : phase.systems) {
        if (system.enabled) {
          system.system(context, dt);
        }
      }
    }

    if (phase.dispatchAfter) {
      context.getEventBus().dispatch();
    }

    if (phase.flushAfter) {
      context.getCommandBuffer().flush();
    }
  }
}

void ECSScheduler::updateOneShotPhase(const std::string &phase, float dt) {
  const auto it{oneShotPhases.find(phase)};

  if (it == oneShotPhases.end()) {
    return;
  }

  for (const auto &system : it->second.systems) {
    if (system.enabled) {
      system.system(context, dt);
    }
  }

  if (it->second.dispatchAfter) {
    context.getEventBus().dispatch();
  }

  if (it->second.flushAfter) {
    context.getCommandBuffer().flush();
  }
}

void ECSScheduler::addPhase(std::string phase, bool dispatchAfter,
                            bool flushAfter) {
  auto &phaseSlot{phases.emplace_back(PhaseSlot{
      phase, std::vector<SystemSlot>(), dispatchAfter, flushAfter, true})};

  rebuildLookUpByEntity();
  rebuildLookUpByPhase();

  lookUpByPhase[std::move(phase)] = &phaseSlot;
}

void ECSScheduler::addOneShotPhase(std::string phase, bool dispatchAfter,
                                   bool flushAfter) {
  auto [it, _]{oneShotPhases.emplace(
      phase, PhaseSlot{phase, {}, dispatchAfter, flushAfter, true})};

  rebuildLookUpByEntity();
  rebuildLookUpByPhase();

  lookUpByPhase[phase] = &it->second;
}

void ECSScheduler::removePhase(const std::string &phase) {
  phases.erase(std::remove_if(phases.begin(), phases.end(),
                              [&phaseId = phase](const auto &phase) {
                                return phase.phaseId == phaseId;
                              }),
               phases.end());

  oneShotPhases.erase(phase);

  rebuildLookUpByEntity();
  rebuildLookUpByPhase();
}

void ECSScheduler::registerSystem(
    const std::string &phase, std::string systemId,
    std::function<void(ECSContext &, const float)> system) {
  auto it{lookUpByPhase.find(phase)};

  if (it == lookUpByPhase.end()) {
    return;
  }

  auto sysSlot{it->second->systems.emplace_back(
      SystemSlot{systemId, std::move(system), true})};

  rebuildLookUpByEntity();

  lookUpByEntity[std::move(systemId)] =
      std::make_pair(&(*it->second), &sysSlot);
}

void ECSScheduler::removeSystem(const std::string &systemId) {
  const auto it{lookUpByEntity.find(systemId)};

  if (it == lookUpByEntity.end()) {
    return;
  }

  auto &systems{it->second.first->systems};
  auto sysPtr = it->second.second;

  systems.erase(
      std::remove_if(systems.begin(), systems.end(),
                     [&sysPtr](auto &system) { return &system == sysPtr; }),
      systems.end());

  rebuildLookUpByEntity();
}

void ECSScheduler::enableSystem(const std::string &id) {
  const auto it{lookUpByEntity.find(id)};

  if (it == lookUpByEntity.end()) {
    return;
  }

  it->second.second->enabled = true;
}

void ECSScheduler::disableSystem(const std::string &id) {
  const auto it{lookUpByEntity.find(id)};

  if (it == lookUpByEntity.end()) {
    return;
  }

  it->second.second->enabled = false;
}

void ECSScheduler::enablePhase(const std::string &id) {
  const auto it{lookUpByPhase.find(id)};

  if (it == lookUpByPhase.end()) {
    return;
  }

  it->second->enabled = true;
}

void ECSScheduler::disablePhase(const std::string &id) {
  const auto it{lookUpByPhase.find(id)};

  if (it == lookUpByPhase.end()) {
    return;
  }

  it->second->enabled = false;
}

void ECSScheduler::rebuildLookUpByEntity() {
  lookUpByEntity.clear();

  for (auto &phase : phases) {
    for (auto &system : phase.systems) {
      lookUpByEntity[system.systemId] = std::make_pair(&phase, &system);
    }
  }

  for (auto &[_, phase] : oneShotPhases) {
    for (auto &system : phase.systems) {
      lookUpByEntity[system.systemId] = std::make_pair(&phase, &system);
    }
  }
}

void ECSScheduler::rebuildLookUpByPhase() {
  lookUpByPhase.clear();

  for (auto &phase : phases) {
    lookUpByPhase[phase.phaseId] = &phase;
  }

  for (auto &[_, phase] : oneShotPhases) {
    lookUpByPhase[phase.phaseId] = &phase;
  }
}
