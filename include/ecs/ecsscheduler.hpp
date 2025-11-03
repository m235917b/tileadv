#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ECSContext;

struct SystemSlot {
  std::string systemId;
  std::function<void(ECSContext &, const float dt)> system;
  bool enabled;
};

struct PhaseSlot {
  std::string phaseId;
  std::vector<SystemSlot> systems;
  bool dispatchAfter;
  bool flushAfter;
  bool enabled;
};

class ECSScheduler {
public:
  ECSScheduler(ECSContext &context);
  ~ECSScheduler() = default;

  void bootstrap();
  void update(float dt);
  void updateOneShotPhase(const std::string &phase, float dt);
  void addPhase(std::string phase, bool dispathcAfter, bool flushAfter);
  void addOneShotPhase(std::string phase, bool dispathcAfter, bool flushAfter);
  void removePhase(const std::string &phase);
  void registerSystem(const std::string &phase, std::string systemId,
                      std::function<void(ECSContext &, const float dt)> system);
  void removeSystem(const std::string &systemId);
  void enableSystem(const std::string &id);
  void disableSystem(const std::string &id);
  void enablePhase(const std::string &id);
  void disablePhase(const std::string &id);

private:
  ECSContext &context;
  std::vector<PhaseSlot> phases;
  std::unordered_map<std::string, std::pair<PhaseSlot *, SystemSlot *>>
      lookUpByEntity;
  std::unordered_map<std::string, PhaseSlot *> lookUpByPhase;
  std::unordered_map<std::string, PhaseSlot> oneShotPhases;

  void rebuildLookUpByEntity();
  void rebuildLookUpByPhase();
};
