#pragma once

#include <any>
#include <functional>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ecs/ecsfunctionaliases.hpp"

class ECSContext;

class ECSEventBus {
public:
  ECSEventBus(ECSContext &context);
  ~ECSEventBus() = default;

  void subscribe(const std::type_index &type, EventListenerAny listener);
  void publish(std::any event);
  void dispatch();

  template <typename EventType>
  void subscribe(EventHandler<EventType> listener) {
    const auto wrap{[listener = std::move(listener)](ECSContext &context,
                                                     const std::any &eventAny) {
      listener(context, std::any_cast<const EventType &>(eventAny));
    }};

    subscribe(std::type_index(typeid(EventType)), std::move(wrap));
  }

  template <typename EventType> void publish(EventType event) {
    publish(std::move(std::make_any<EventType>(event)));
  }

private:
  ECSContext &context;
  std::queue<std::any> queue;
  std::unordered_map<std::type_index, std::vector<EventListenerAny>> listeners;
  bool inDispatch;
};
