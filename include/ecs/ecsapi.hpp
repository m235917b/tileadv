#pragma once

#include "ecs/ecscontext.hpp"

class ECSAPI {
public:
  ECSAPI(ECSContext &context);
  ~ECSAPI() = default;

  void
  view(const std::vector<std::type_index> &types,
       const std::function<void(const std::string &,
                                const std::vector<const std::any *> &)> &f);
  void addViewSystem(const std::string &phase, std::string systemId,
                     std::vector<std::type_index> types,
                     std::function<void(const std::string &,
                                        const std::vector<const std::any *> &)>
                         system);

  template <typename... Ts, typename Func> void view(Func &&f) {
    context.getStore().view<Ts...>(std::forward<Func>(f));
  }

  template <typename... Ts, typename Func>
  void addViewSystem(const std::string &phase, std::string systemId,
                     Func &&system) {
    std::vector<std::type_index> types;
    types.reserve(sizeof...(Ts));
    (types.emplace_back(std::type_index(typeid(std::decay_t<Ts>))), ...);

    const auto wrap{[system = std::forward<Func>(system)](
                        const std::string &entityId,
                        const std::vector<const std::any *> &components) {
      auto apply{[&]<std::size_t... Is>(std::index_sequence<Is...>) {
        system(entityId, (std::any_cast<const Ts &>(*components[Is]))...);
      }};
      apply(std::make_index_sequence<sizeof...(Ts)>{});
    }};

    addViewSystem(phase, std::move(systemId), std::move(types),
                  std::move(wrap));
  }

private:
  ECSContext &context;
};
