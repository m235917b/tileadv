#include <algorithm>

#include "ecs/ecscontext.hpp"
#include "ecs/ecsscheduler.hpp"

ECSScheduler::ECSScheduler(ECSContext &context)
    : context(context), phases(), lookUp() {}

void ECSScheduler::bootstrap() {
  context.getEventBus().dispatch();
  context.getCommandBuffer().flush();
}

void ECSScheduler::update(const float dt) {
  for (const auto &phase : phases) {
    for (const auto &system : phase.systems) {
      if (system.enabled) {
        system.system(context, dt);
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

void ECSScheduler::addPhase(std::string phase, bool dispatchAfter,
                            bool flushAfter) {
  phases.push_back(PhaseSlot{std::move(phase), std::vector<SystemSlot>(),
                             dispatchAfter, flushAfter});
}

void ECSScheduler::removePhase(const std::string &phase) {
  phases.erase(std::remove_if(phases.begin(), phases.end(),
                              [&phaseId = phase](const auto &phase) {
                                return phase.phaseId == phaseId;
                              }),
               phases.end());

  rebuildLookUp();
}

void ECSScheduler::registerSystem(
    const std::string &phase, std::string systemId,
    std::function<void(ECSContext &, const float)> system) {
  const auto it{std::find_if(phases.begin(), phases.end(),
                             [&phaseExt = phase](const auto &phase) {
                               return phase.phaseId == phaseExt;
                             })};

  if (it == phases.end()) {
    return;
  }

  it->systems.push_back(SystemSlot{systemId, std::move(system), true});
  lookUp[std::move(systemId)] = std::make_pair(&(*it), &it->systems.back());
}

void ECSScheduler::removeSystem(const std::string &systemId) {
  const auto it{lookUp.find(systemId)};

  if (it == lookUp.end()) {
    return;
  }

  auto &systems{it->second.first->systems};
  auto sysPtr = it->second.second;

  systems.erase(
      std::remove_if(systems.begin(), systems.end(),
                     [&sysPtr](auto &system) { return &system == sysPtr; }),
      systems.end());

  rebuildLookUp();
}

void ECSScheduler::enableSystem(const std::string &id) {
  const auto it{lookUp.find(id)};

  if (it == lookUp.end()) {
    return;
  }

  it->second.second->enabled = true;
}

void ECSScheduler::disableSystem(const std::string &id) {
  const auto it{lookUp.find(id)};

  if (it == lookUp.end()) {
    return;
  }

  it->second.second->enabled = false;
}

void ECSScheduler::rebuildLookUp() {
  lookUp.clear();

  for (auto &phase : phases) {
    for (auto &system : phase.systems) {
      lookUp[system.systemId] = std::make_pair(&phase, &system);
    }
  }
}
