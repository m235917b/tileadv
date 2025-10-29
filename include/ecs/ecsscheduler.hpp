#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ECSContext;

using SystemFn = std::function<void(ECSContext &, float dt)>;

class ECSScheduler {
public:
  ECSScheduler(ECSContext &context);
  ~ECSScheduler() = default;

  void update(float dt);
  void addPhase(std::string phase);
  void removePhase(const std::string &phase);
  void addSystem(const std::string &phase, std::string systemId,
                 SystemFn system);
  void removeSystem(const std::string &phase, const std::string &systemId);

private:
  ECSContext &context;
  std::vector<std::string> phases;
  std::unordered_map<std::string, std::vector<std::pair<std::string, SystemFn>>>
      systems;
};
