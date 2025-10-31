#include "ecs/ecseventbus.hpp"

ECSEventBus::ECSEventBus(ECSContext &context)
    : context(context), queue(), listeners(), inDispatch(false) {}

void ECSEventBus::subscribe(
    const std::type_index &type,
    std::function<void(ECSContext &, const std::any &)> listener) {
  if (inDispatch) {
    return;
  }

  listeners[type].emplace_back(std::move(listener));
}

void ECSEventBus::publish(std::any event) { queue.emplace(std::move(event)); }

void ECSEventBus::dispatch() {
  if (inDispatch) {
    return;
  }

  inDispatch = true;

  while (!queue.empty()) {
    const auto &event{queue.front()};
    const auto eventType{std::type_index(event.type())};

    const auto it{listeners.find(eventType)};
    if (it != listeners.end()) {
      for (const auto &listener : it->second) {
        listener(context, event);
      }
    }

    queue.pop();
  }

  inDispatch = false;
}
