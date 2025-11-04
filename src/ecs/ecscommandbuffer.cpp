#include "ecs/ecscommandbuffer.hpp"

#include <iostream>

#include "ecs/ecscontext.hpp"

struct PrintCommand {
  std::string text;
};

struct UpsertComponent {
  std::string entityId;
  std::any payload;
};

struct UpsertResource {
  std::any payload;
};

struct PatchComponent {
  std::string entityId;
  std::type_index componentType;
  std::function<void(std::any &)> setter;
};

struct PatchResource {
  std::type_index resourceType;
  std::function<void(std::any &)> setter;
};

ECSCommandBuffer::ECSCommandBuffer(ECSContext &context)
    : context(context), queue(), handlers(), inFlush(false),
      reservedCommands() {
  reservedCommands.emplace(std::type_index(typeid(PrintCommand)));
  reservedCommands.emplace(std::type_index(typeid(UpsertComponent)));
  reservedCommands.emplace(std::type_index(typeid(UpsertResource)));
  reservedCommands.emplace(std::type_index(typeid(PatchComponent)));
  reservedCommands.emplace(std::type_index(typeid(PatchResource)));

  registerHandlerInternal<PrintCommand>([](ECSContext &, PrintCommand command) {
    std::cout << command.text << std::endl;
  });

  registerHandlerInternal<UpsertComponent>(
      [this](ECSContext &, UpsertComponent command) {
        this->context.store.upsertComponent(command.entityId,
                                            std::move(command.payload));
      });

  registerHandlerInternal<UpsertResource>([this](ECSContext &,
                                                 UpsertResource command) {
    this->context.resourceManager.setResource(
        std::type_index(command.payload.type()), std::move(command.payload));
  });

  registerHandlerInternal<PatchComponent>([this](ECSContext &,
                                                 PatchComponent command) {
    const auto &comp{this->context.store.getComponent(command.entityId,
                                                      command.componentType)};

    if (!comp) {
      return;
    }

    command.setter(*comp);
  });

  registerHandlerInternal<PatchResource>(
      [this](ECSContext &, PatchResource command) {
        const auto &comp{
            this->context.resourceManager.getResource(command.resourceType)};

        if (!comp) {
          return;
        }

        command.setter(*comp);
      });
}

void ECSCommandBuffer::registerHandler(
    const std::type_index &type,
    std::function<void(ECSContext &, const std::any &)> handler) {
  if (inFlush || reservedCommands.contains(type)) {
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

void ECSCommandBuffer::print(std::string text) {
  enqueue(std::make_any<PrintCommand>(PrintCommand{std::move(text)}));
}

void ECSCommandBuffer::upsertComponent(std::string entityId,
                                       std::any component) {
  enqueue(std::make_any<UpsertComponent>(
      UpsertComponent{std::move(entityId), std::move(component)}));
}

void ECSCommandBuffer::upsertResource(std::any component) {
  enqueue(std::make_any<UpsertResource>(UpsertResource{std::move(component)}));
}

void ECSCommandBuffer::patchComponent(std::string entityId,
                                      std::type_index type,
                                      std::function<void(std::any &)> setter) {
  enqueue(std::make_any<PatchComponent>(
      PatchComponent{std::move(entityId), type, std::move(setter)}));
}

void ECSCommandBuffer::patchResource(std::type_index type,
                                     std::function<void(std::any &)> setter) {
  enqueue(std::make_any<PatchResource>(PatchResource{type, std::move(setter)}));
}
