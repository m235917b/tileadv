#pragma once

#include <any>
#include <functional>
#include <string>
#include <typeindex>
#include <vector>

#include "ecs/ecsentitybuilder.hpp"
#include "ecs/ecsprefab.hpp"

class ECSContext;

class ECSAPI {
public:
  ECSAPI(ECSContext &context);
  ~ECSAPI() = default;

  ECSPrefab &getPrefab();

  ECSEntityBuilder createEntity(std::string entityId);
  ECSEntityBuilder instantiateEntity(std::string entityId,
                                     std::string recipeId);

  void addViewSystem(
      const std::string &phase, std::string systemId,
      std::vector<std::type_index> types,
      std::function<void(ECSContext &, const float, const std::string &,
                         const std::vector<const std::any *> &)>
          system);
  void subscribeViewEventListener(
      const std::type_index &type, std::vector<std::type_index> types,
      std::function<void(ECSContext &, const std::any &, const std::string &,
                         const std::vector<const std::any *> &)>
          listener);

  template <typename... Ts, typename Func>
  void addViewSystem(const std::string &phase, std::string systemId,
                     Func &&system) {
    std::vector<std::type_index> types;
    types.reserve(sizeof...(Ts));
    (types.emplace_back(std::type_index(typeid(std::decay_t<Ts>))), ...);

    const auto wrap{[system = std::forward<Func>(system)](
                        ECSContext &context, const float dt,
                        const std::string &entityId,
                        const std::vector<const std::any *> &components) {
      auto apply{[&]<std::size_t... Is>(std::index_sequence<Is...>) {
        system(context, dt, entityId,
               (std::any_cast<const Ts &>(*components[Is]))...);
      }};
      apply(std::make_index_sequence<sizeof...(Ts)>{});
    }};

    addViewSystem(phase, std::move(systemId), std::move(types),
                  std::move(wrap));
  }

  template <typename EventType, typename... Ts, typename Func>
  void subscribeViewListener(Func &&listener) {
    std::vector<std::type_index> types;
    types.reserve(sizeof...(Ts));
    (types.emplace_back(std::type_index(typeid(std::decay_t<Ts>))), ...);

    const auto wrap{[listener = std::forward<Func>(listener)](
                        ECSContext &context, const std::any &event,
                        const std::string &entityId,
                        const std::vector<const std::any *> &components) {
      auto apply{[&]<std::size_t... Is>(std::index_sequence<Is...>) {
        listener(context, std::any_cast<const EventType &>(event), entityId,
                 (std::any_cast<const Ts &>(*components[Is]))...);
      }};
      apply(std::make_index_sequence<sizeof...(Ts)>{});
    }};

    subscribeViewEventListener(std::type_index(typeid(EventType)),
                               std::move(types), std::move(wrap));
  }

private:
  ECSContext &context;
  ECSPrefab prefab;
};
