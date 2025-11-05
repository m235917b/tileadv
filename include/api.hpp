#pragma once

#include <functional>
#include <string>

#include "asc/asc.hpp"
#include "asc/events.hpp"
#include "asc/resources.hpp"
#include "ecs/ecs.hpp"
#include "engine/engine.hpp"
#include "engine/resources.hpp"

template <typename EventType>
using EventConditionFn = std::function<bool(const EventType &event)>;

template <typename EventType>
using EventPayloadFn = std::function<std::any(const EventType &event)>;

class GameAPI {
public:
  GameAPI() = default;
  ~GameAPI() = default;

  void run();

  void registerEntityEffect(std::string id, EntityEffectFn effect);

  template <typename EventType>
  void registerEventEffectTrigger(std::string effectId,
                                  EventConditionFn<EventType> condition,
                                  EventPayloadFn<EventType> payload) {
    asc.getECSContext().getEventBus().subscribe<EventType>(
        [effectId = std::move(effectId), condition = std::move(condition),
         payload = std::move(payload)](ECSContext &ecsContext,
                                       const EventType &event) {
          if (condition(event)) {
            auto &api{*ecsContext.getResourceManager()
                           .getResource<ECSAPIResource>()
                           ->ecsApi};
            const auto &table{ecsContext.getResourceManager()
                                  .getResource<EntityEffectTableResource>()
                                  ->table};
            const auto it{table.find(effectId)};

            if (it == table.end()) {
              return;
            }

            it->second.apply(api, payload(event));
          }
        });
  }

private:
  ASC asc;
};
