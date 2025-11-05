#pragma once

#include <any>
#include <functional>
#include <optional>
#include <string>
#include <typeindex>

#include "actor/components.hpp"
#include "asc/asc.hpp"
#include "asc/events.hpp"
#include "asc/resources.hpp"
#include "ecs/ecs.hpp"
#include "engine/engine.hpp"
#include "engine/resources.hpp"

template <typename EventType>
using EventPayloadFn =
    std::function<std::optional<std::any>(const EventType &event)>;

using EventPayloadAnyFn =
    std::function<std::optional<std::any>(const std::any &event)>;

using PayloadFn = std::function<std::optional<std::any>(GameAPI &)>;

using UpdateAny = std::function<void(GameAPI &, const std::string &,
                                     const std::vector<const std::any *> &)>;

class GameAPI {
public:
  GameAPI();
  ~GameAPI() = default;

  void run();
  void registerEntityEffect(std::string id, EntityEffectFn effect);
  void registerEventEffectTrigger(std::type_index eventType,
                                  std::string effectId,
                                  EventPayloadAnyFn payload);
  void registerUpdate(std::vector<std::type_index> types, UpdateAny update);
  void print(std::string text);
  ECSPrefab &getPrefab();
  ECSEntityBuilder createEntity(std::string entityId);
  ECSEntityBuilder instantiateEntity(std::string entityId,
                                     std::string recipeId);
  void upsertComponent(std::string entityId, std::any component);
  const std::any *getComponent(const std::string &entityId,
                               const std::type_index &type);

  template <typename EventType>
  void registerEventEffectTrigger(std::string effectId,
                                  EventPayloadFn<EventType> payload) {
    auto payloadWrapper{[payload = std::move(payload)](const std::any &event) {
      return payload(std::any_cast<EventType>(event));
    }};

    registerEventEffectTrigger(std::type_index(typeid(EventType)),
                               std::move(effectId), std::move(payloadWrapper));
  }

  template <typename... ComponentType, typename Update>
  void registerUpdate(Update &&update) {
    static_assert(sizeof...(ComponentType) > 0);

    std::vector<std::type_index> types;
    types.reserve(sizeof...(ComponentType));
    (types.emplace_back(std::type_index(typeid(std::decay_t<ComponentType>))),
     ...);

    const auto wrap{[f = std::move(update)](
                        GameAPI &ctxApi, const std::string &entityId,
                        const std::vector<const std::any *> &components) {
      auto apply{[&]<std::size_t... Is>(std::index_sequence<Is...>) {
        f(ctxApi, entityId,
          (std::any_cast<const ComponentType &>(*components[Is]))...);
      }};
      apply(std::make_index_sequence<sizeof...(ComponentType)>{});
    }};

    registerUpdate(types, wrap);
  }

  template <typename T>
  void upsertComponent(std::string entityId, T component) {
    upsertComponent(std::move(entityId), std::make_any<T>(component));
  }

  template <typename T> const T *getComponent(const std::string &entityId) {
    return std::any_cast<T>(getComponent(entityId, std::type_index(typeid(T))));
  }

private:
  ASC asc;
  int systemFnId;
};
