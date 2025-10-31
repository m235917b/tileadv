#include "ecs/ecscommandbuffer.hpp"

ECSCommandBuffer::ECSCommandBuffer(ECSContext &context)
    : context(context), queue(), handlers(), inFlush(false) {}

void ECSCommandBuffer::registerHandler(
    const std::type_index &type,
    std::function<void(ECSContext &, const std::any &)> handler) {
  if (inFlush || reserved_commands.contains(type)) {
    return;
  }

  handlers[type] = std::move(handler);
}

void ECSCommandBuffer::enqueue(std::any command) {
  queue.emplace(std::move(command));
}

void ECSCommandBuffer::flush() {
  if (inFlush) {
    return;
  }

  inFlush = true;

  while (!queue.empty()) {
    auto command{std::move(queue.front())};
    const auto commandType{std::type_index(command.type())};

    const auto it{handlers.find(commandType)};

    if (it != handlers.end()) {
      it->second(context, std::move(command));
    }

    queue.pop();
  }

  inFlush = false;
}

void ECSCommandBuffer::registerHandlerInternal(
    const std::type_index &type,
    std::function<void(ECSContext &, std::any)> handler) {
  if (inFlush) {
    return;
  }

  handlers[type] = std::move(handler);
}
