#pragma once

#include <any>
#include <functional>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

class ECSContext;

class ECSEventBus {
public:
  ECSEventBus(ECSContext &context) : context(context), queue(), listeners() {}
  ~ECSEventBus() = default;

  template <typename EventType>
  void
  subscribe(std::function<void(ECSContext &, const EventType &)> listener) {
    auto wrap = [listener = std::move(listener)](ECSContext &context,
                                                 const std::any &eventAny) {
      listener(context, std::any_cast<const EventType &>(eventAny));
    };
    listeners[std::type_index(typeid(EventType))].emplace_back(std::move(wrap));
  }

  template <typename EventType> void publish(EventType &&event) {
    queue.emplace(std::forward<EventType>(event));
  }

  void dispatch() {
    while (!queue.empty()) {
      const auto &event = queue.front();
      const auto eventType = std::type_index(event.type());

      auto it = listeners.find(eventType);
      if (it != listeners.end()) {
        for (const auto &listener : it->second) {
          listener(context, event);
        }
      }

      queue.pop();
    }
  }

private:
  ECSContext &context;
  std::queue<std::any> queue;
  std::unordered_map<
      std::type_index,
      std::vector<std::function<void(ECSContext &, const std::any &)>>>
      listeners;
};
