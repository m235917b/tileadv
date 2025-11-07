#pragma once

#include <any>
#include <functional>
#include <optional>
#include <string>
#include <typeindex>

#include "actor/components.hpp"
#include "actor/events.hpp"
#include "asc/asc.hpp"
#include "asc/events.hpp"
#include "asc/resources.hpp"
#include "ecs/ecs.hpp"
#include "engine/engine.hpp"
#include "engine/resources.hpp"
#include "gui/gui.hpp"

// TODO: Wrap GameAPI parameter in interface to hide register functions

template <typename EventType>
using EventPayloadFn =
    std::function<std::optional<std::any>(const EventType &event)>;

using EventPayloadAnyFn =
    std::function<std::optional<std::any>(const std::any &event)>;

using PayloadFn = std::function<std::optional<std::any>(GameAPI &)>;

// TODO: Think about making this API more restrictive!
using UpdateAny = std::function<void(GameAPI &, float dt, const std::string &,
                                     const std::vector<const std::any *> &)>;

class GameAPI {
public:
  GameAPI();
  ~GameAPI() = default;

  void run();

  void registerEffect(std::string id, EntityEffectFn effect);
  void registerEventEffectTrigger(std::type_index eventType,
                                  std::string effectId,
                                  EventPayloadAnyFn payload);
  void registerUpdate(std::vector<std::type_index> types, UpdateAny update);
  ECSPrefab &getPrefab();
  // TODO: Return ID of created entity
  ECSEntityBuilder instantiateEntity(std::string recipeId);
  void upsertComponent(std::string entityId, std::any component);
  const std::any *getComponent(const std::string &entityId,
                               const std::type_index &type);
  const std::any *getResource(const std::type_index &type);
  void publishEvent(std::any event);
  bool hasComponent(std::type_index type, const std::string &id);
  void setApplicationState(ApplicationState state);

  GUIContext &getGUIContext();

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
                        GameAPI &ctxApi, float dt, const std::string &entityId,
                        const std::vector<const std::any *> &components) {
      auto apply{[&]<std::size_t... Is>(std::index_sequence<Is...>) {
        f(ctxApi, dt, entityId,
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

  template <typename EventType> void publishEvent(EventType event) {
    publishEvent(std::make_any<EventType>(event));
  }

  template <typename ComponentType> bool hasComponent(const std::string &id) {
    return hasComponent(std::type_index(typeid(ComponentType)), id);
  }

  std::pair<float, float> getMousePos();
  std::pair<float, float> getMouseTile();
  void print(std::string text);

private:
  ASC asc;
  int systemFnId;
  int entityId;
};
