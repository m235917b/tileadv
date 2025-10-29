#include <algorithm>

#include "ecs/ecsscheduler.hpp"

ECSScheduler::ECSScheduler(ECSContext &context)
    : context(context), phases(), systems() {}

void ECSScheduler::update(float dt) {
  for (auto &phase : phases) {
    auto it = systems.find(phase);
    if (it != systems.end()) {
      for (auto &system : it->second) {
        system.second(context, dt);
      }
    }
  }
}

void ECSScheduler::addPhase(std::string phase) {
  phases.push_back(std::move(phase));
}

void ECSScheduler::removePhase(const std::string &phase) {
  phases.erase(std::remove(phases.begin(), phases.end(), phase), phases.end());
}

void ECSScheduler::addSystem(const std::string &phase, std::string systemId,
                             SystemFn system) {
  systems[phase].emplace_back(std::move(systemId), std::move(system));
}

void ECSScheduler::removeSystem(const std::string &phase,
                                const std::string &systemId) {
  auto it = systems.find(phase);

  if (it != systems.end()) {
    auto &sysVec = it->second;
    sysVec.erase(
        std::remove_if(sysVec.begin(), sysVec.end(),
                       [&](const auto &p) { return p.first == systemId; }),
        sysVec.end());
  }
}
