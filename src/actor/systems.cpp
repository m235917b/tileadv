#include "actor/systems.hpp"

#include <any>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "actor/components.hpp"
#include "actor/resources.hpp"
#include "chunk/events.hpp"
#include "chunk/resources.hpp"

struct EntityPosMap {
  std::vector<const std::string *> entityMap;
  std::unordered_map<std::string, std::pair<int, int>> posMap;
};

inline EntityPosMap buildEntityPosMap(ECSContext &ecsContext, int chunkSizeX,
                                      int chunkSizeY) {
  EntityPosMap map;
  map.entityMap.resize(chunkSizeX * chunkSizeY, nullptr);

  ecsContext.getStore().view(
      std::vector<std::type_index>{std::type_index(typeid(Position))},
      [&](const std::string &id,
          const std::vector<const std::any *> &components) {
        const auto pos{std::any_cast<Position>(components.at(0))};
        map.entityMap[pos->y * chunkSizeX + pos->x] = &id;
        map.posMap[id] = std::make_pair(pos->x, pos->y);
      });

  return map;
}

inline void updateDirection(ECSContext &context, const MoveIntentEvent &event,
                            const Chunk &chunk, EntityPosMap &map, int oldX,
                            int oldY, int newX, int newY) {
  if (newY < 0) {
    context.getEventBus().publish(
        std::make_any<ChunkBorderEvent>(ChunkBorderEvent{ChunkBorder::NORTH}));
  } else if (newX < 0) {
    context.getEventBus().publish(
        std::make_any<ChunkBorderEvent>(ChunkBorderEvent{ChunkBorder::WEST}));
  } else if (newY >= chunk.sizeY) {
    context.getEventBus().publish(
        std::make_any<ChunkBorderEvent>(ChunkBorderEvent{ChunkBorder::SOUTH}));
  } else if (newX >= chunk.sizeX) {
    context.getEventBus().publish(
        std::make_any<ChunkBorderEvent>(ChunkBorderEvent{ChunkBorder::EAST}));
  } else {
    const auto &tile{getTileAt(chunk, newX, newY)};
    if (!tile.solid) {
      const auto &other{map.entityMap.at(newY * chunk.sizeX + newX)};
      if (other == nullptr) {
        if (oldX != newX) {
          context.getCommandBuffer().patchComponent<Position>(
              event.entityId, &Position::x, newX);
        }
        if (oldY != newY) {
          context.getCommandBuffer().patchComponent<Position>(
              event.entityId, &Position::y, newY);
        }
        map.posMap[event.entityId] = std::make_pair(newX, newY);
        map.entityMap[newY * chunk.sizeX + newX] = &event.entityId;
        map.entityMap[oldY * chunk.sizeX + oldX] = nullptr;
      } else {
        context.getEventBus().publish(std::make_any<ActorCollisionEvent>(
            ActorCollisionEvent{event.entityId, *other}));
      }
    } else {
      context.getEventBus().publish(std::make_any<WorldCollisionEvent>(
          WorldCollisionEvent{event.entityId}));
    }
  }
}

const auto collisionSystem{[](ECSContext &ecsContext, const float) {
  const auto chunk{
      std::any_cast<Chunk>(ecsContext.getResourceManager().getResource(
          std::type_index(typeid(Chunk))))};
  auto map = buildEntityPosMap(ecsContext, chunk->sizeX, chunk->sizeY);
  const auto &moveIntents{
      std::any_cast<MoveIntentEventQueue>(
          ecsContext.getResourceManager().getResource(
              std::type_index(typeid(MoveIntentEventQueue))))
          ->moveIntents};

  for (const auto &moveIntent : moveIntents) {
    const auto &pos{map.posMap[moveIntent.entityId]};
    switch (moveIntent.dir) {
    case Direction::UP:
      updateDirection(ecsContext, moveIntent, *chunk, map, pos.first,
                      pos.second, pos.first, pos.second - 1);
      break;
    case Direction::LEFT:
      updateDirection(ecsContext, moveIntent, *chunk, map, pos.first,
                      pos.second, pos.first - 1, pos.second);
      break;
    case Direction::DOWN:
      updateDirection(ecsContext, moveIntent, *chunk, map, pos.first,
                      pos.second, pos.first, pos.second + 1);
      break;
    case Direction::RIGHT:
      updateDirection(ecsContext, moveIntent, *chunk, map, pos.first,
                      pos.second, pos.first + 1, pos.second);
      break;
    }
  }

  ecsContext.getCommandBuffer().patchResource(
      std::type_index(typeid(MoveIntentEventQueue)), [=](std::any &queueAny) {
        auto &queue{std::any_cast<MoveIntentEventQueue &>(queueAny)};
        queue.moveIntents.clear();
      });
}};

const auto garbageCollectorSystem{[](ECSContext &ecsContext, const float) {
  ecsContext.getStore().view<Garbage>(
      [&ecsContext](const std::string &entityId, const Garbage &garb) {
        if (garb.isGarbage) {
          ecsContext.getCommandBuffer().destroyEntity(entityId);
        }
      });
}};

std::vector<SystemRegEntry> getActorSystems() {
  return {SystemRegEntry{"collision", collisionSystem},
          SystemRegEntry{"garbage_collector", garbageCollectorSystem}};
}
