#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ECSContext;

class ECSScheduler {
public:
  ECSScheduler(ECSContext &context);
  ~ECSScheduler() = default;

  void bootstrap();
  void update(float dt);
  void addPhasePre(std::string phase);
  void addPhasePost(std::string phase);
  void removePhase(const std::string &phase);
  void registerSystem(const std::string &phase, std::string systemId,
                 std::function<void(ECSContext &, const float dt)> system);
  void removeSystem(const std::string &systemId);
  void enableSystem(const std::string &id);
  void disableSystem(const std::string &id);

private:
  ECSContext &context;
  std::vector<std::string> phasesPre;
  std::vector<std::string> phasesPost;
  std::unordered_map<
      std::string,
      std::unordered_map<std::string,
                         std::function<void(ECSContext &, const float dt)>>>
      systems;
  std::unordered_map<std::string, bool> enabled;
  std::unordered_map<std::string, std::string> idToPhase;
};
