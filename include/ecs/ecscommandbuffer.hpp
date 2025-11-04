#pragma once

#include <any>
#include <functional>
#include <queue>
#include <set>
#include <typeindex>
#include <unordered_map>

#include "ecs/ecsfunctionaliases.hpp"

class ECSContext;

class ECSCommandBuffer {
public:
  ECSCommandBuffer(ECSContext &context);
  ~ECSCommandBuffer() = default;

  void registerHandler(const std::type_index &type, CommandHandlerAny handler);
  void enqueue(std::any command);
  void flush();
  void print(std::string text);
  void upsertComponent(std::string entityId, std::any component);
  void upsertResource(std::any component);

  template <typename CommandType>
  void registerHandler(CommandHandler<CommandType> handler) {
    auto wrap{[handler = std::move(handler)](ECSContext &context,
                                             const std::any &command) {
      handler(context, std::move(std::any_cast<CommandType>(command)));
    }};
    registerHandler(std::type_index(typeid(CommandType)), std::move(wrap));
  }

  template <typename CommandType> void enqueue(CommandType command) {
    enqueue(std::move(std::make_any<CommandType>(command)));
  }

  template <typename ComponentType>
  void upsertComponent(std::string entityId, ComponentType component) {
    upsertComponent(std::move(entityId),
                    std::make_any<ComponentType>(std::move(component)));
  }

  template <typename ResourceType> void upsertResource(ResourceType resource) {
    upsertResource(std::make_any<ResourceType>(std::move(resource)));
  }

  template <typename ComponentType, typename FieldType>
  void patchComponent(std::string entityId, FieldType ComponentType::*field,
                      FieldType val) {
    auto setter{[v = std::move(val), field](std::any &comp) {
      std::any_cast<ComponentType &>(comp).*field = std::move(v);
    }};

    patchComponent(std::move(entityId), std::type_index(typeid(ComponentType)),
                   setter);
  }

  template <typename ComponentType, typename FieldType>
  void patchResource(FieldType ComponentType::*field, FieldType val) {
    auto setter{[v = std::move(val), field](std::any &res) {
      std::any_cast<ComponentType &>(res).*field = std::move(v);
    }};

    patchResource(std::type_index(typeid(ComponentType)), setter);
  }

private:
  ECSContext &context;
  std::queue<std::any> queue;
  std::unordered_map<std::type_index, CommandHandlerAny> handlers;
  bool inFlush;
  std::set<std::type_index> reservedCommands;

  void patchComponent(std::string entityId, std::type_index type,
                      MemberSetter setter);
  void patchResource(std::type_index type, MemberSetter setter);

  void registerHandlerInternal(const std::type_index &type,
                               CommandHandlerAny handler);

  template <typename CommandType>
  void registerHandlerInternal(CommandHandler<CommandType> handler) {
    auto wrap{[handler = std::move(handler)](ECSContext &context,
                                             const std::any &command) {
      handler(context, std::move(std::any_cast<CommandType>(command)));
    }};
    registerHandlerInternal(std::type_index(typeid(CommandType)),
                            std::move(wrap));
  }
};
