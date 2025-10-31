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
  ECSCommandBuffer(ECSContext &context);
  ~ECSCommandBuffer() = default;

  void
  registerHandler(const std::type_index &type,
                  std::function<void(ECSContext &, const std::any &)> handler);
  void enqueue(std::any command);
  void flush();

  template <typename CommandType>
  void registerHandler(std::function<void(ECSContext &, CommandType)> handler) {
    auto wrap{[handler = std::move(handler)](ECSContext &context,
                                             const std::any &command) {
      handler(context, std::move(std::any_cast<CommandType>(command)));
    }};
    registerHandler(std::type_index(typeid(CommandType)), std::move(wrap));
  }

  template <typename CommandType> void enqueue(CommandType command) {
    enqueue(std::move(std::make_any<CommandType>(command)));
  }

private:
  ECSContext &context;
  std::queue<std::any> queue;
  std::unordered_map<std::type_index,
                     std::function<void(ECSContext &, std::any)>>
      handlers;
  bool inFlush;

  void
  registerHandlerInternal(const std::type_index &type,
                          std::function<void(ECSContext &, std::any)> handler);

  template <typename CommandType>
  void registerHandlerInternal(
      std::function<void(ECSContext &, CommandType)> handler) {
    auto wrap{[handler = std::move(handler)](ECSContext &context,
                                             const std::any &command) {
      handler(context, std::move(std::any_cast<CommandType>(command)));
    }};
    registerHandlerInternal(std::type_index(typeid(CommandType)),
                            std::move(wrap));
  }

  friend class ECSContext;
};
