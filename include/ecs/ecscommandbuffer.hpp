#pragma once

#include <any>
#include <functional>
#include <queue>
#include <typeindex>
#include <unordered_map>

#include "ecs/ecscommandtypes.hpp"

class ECSContext;

class ECSCommandBuffer {
public:
  ECSCommandBuffer(ECSContext &context)
      : context(context), queue(), handlers() {}
  ~ECSCommandBuffer() = default;

  template <typename CommandType>
  void registerHandler(std::function<void(ECSContext &, CommandType)> handler) {
    static_assert(!is_reserved_command<CommandType>::value,
                  "Cannot register handler for reserved command type");
    auto wrap = [handler = std::move(handler)](ECSContext &context,
                                               std::any commandAny) {
      handler(context, std::move(std::any_cast<CommandType>(commandAny)));
    };
    handlers[std::type_index(typeid(CommandType))] = std::move(wrap);
  }

  template <typename CommandType> void enqueue(CommandType &&command) {
    queue.emplace(std::forward<CommandType>(command));
  }

  void flush() {
    while (!queue.empty()) {
      auto command = std::move(queue.front());
      const auto commandType = std::type_index(command.type());

      auto it = handlers.find(commandType);
      if (it != handlers.end()) {
        it->second(context, std::move(command));
      }

      queue.pop();
    }
  }

private:
  ECSContext &context;
  std::queue<std::any> queue;
  std::unordered_map<std::type_index,
                     std::function<void(ECSContext &, std::any)>>
      handlers;

  template <typename CommandType>
  void registerHandlerInternal(
      std::function<void(ECSContext &, CommandType)> handler) {
    auto wrap = [handler = std::move(handler)](ECSContext &context,
                                               std::any commandAny) {
      handler(context, std::move(std::any_cast<CommandType>(commandAny)));
    };
    handlers[std::type_index(typeid(CommandType))] = std::move(wrap);
  }

  friend class ECSContext;
};
