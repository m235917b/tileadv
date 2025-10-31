#include "ecs/ecsapi.hpp"

ECSAPI::ECSAPI(ECSContext &context) : context(context) {}

void ECSAPI::view(
    const std::vector<std::type_index> &types,
    const std::function<void(const std::string &,
                             const std::vector<const std::any *> &)> &f) {
  context.getStore().view(std::move(types), std::move(f));
}

void ECSAPI::addViewSystem(
    const std::string &phase, std::string systemId,
    std::vector<std::type_index> types,
    std::function<void(const std::string &,
                       const std::vector<const std::any *> &)>
        system) {
  const auto sysFunc{[this, types = std::move(types),
                      system = std::move(system)](ECSContext &, const float) {
    this->context.getStore().view(types, system);
  }};

  context.getScheduler().addSystem(phase, systemId, sysFunc);
}
