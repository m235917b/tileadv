#include "actor/events.hpp"

#include "actor/components.hpp"
#include "chunk/events.hpp"
#include "chunk/resources.hpp"
#include "ecs/ecs.hpp"

void subscribeMoveIntentEventListener(ECSContext &ecsContext) {
  ecsContext.getEventBus().subscribe<MoveIntentEvent>(
      [](ECSContext &context, const MoveIntentEvent &event) {
        const auto &pos{
            context.getStore().getComponent<Position>(event.entityId)};
        const auto &chunk{context.getResourceManager().getResource<Chunk>()};

        switch (event.dir) {
        case Direction::UP:
          if (pos->y <= 0) {
            context.getEventBus().publish(std::make_any<ChunkBorderEvent>(
                ChunkBorderEvent{ChunkBorder::NORTH}));
          } else {
            if (!getTileAt(*chunk, pos->x, pos->y - 1).solid) {
              context.getCommandBuffer().patchComponent<Position>(
                  event.entityId, &Position::y, pos->y - 1);
            } else {
              context.getEventBus().publish(std::make_any<WorldCollisionEvent>(
                  WorldCollisionEvent{event.entityId}));
            }
          }
          break;
        case Direction::LEFT:
          if (pos->x <= 0) {
            context.getEventBus().publish(std::make_any<ChunkBorderEvent>(
                ChunkBorderEvent{ChunkBorder::WEST}));
          } else {
            if (!getTileAt(*chunk, pos->x - 1, pos->y).solid) {
              context.getCommandBuffer().patchComponent<Position>(
                  event.entityId, &Position::x, pos->x - 1);
            } else {
              context.getEventBus().publish(std::make_any<WorldCollisionEvent>(
                  WorldCollisionEvent{event.entityId}));
            }
          }
          break;
        case Direction::DOWN:
          if (pos->y == chunk->sizeY - 1) {
            context.getEventBus().publish(std::make_any<ChunkBorderEvent>(
                ChunkBorderEvent{ChunkBorder::SOUTH}));
          } else {
            if (!getTileAt(*chunk, pos->x, pos->y + 1).solid) {
              context.getCommandBuffer().patchComponent<Position>(
                  event.entityId, &Position::y, pos->y + 1);
            } else {
              context.getEventBus().publish(std::make_any<WorldCollisionEvent>(
                  WorldCollisionEvent{event.entityId}));
            }
          }
          break;
        case Direction::RIGHT:
          if (pos->x >= chunk->sizeX - 1) {
            context.getEventBus().publish(std::make_any<ChunkBorderEvent>(
                ChunkBorderEvent{ChunkBorder::EAST}));
          } else {
            if (!getTileAt(*chunk, pos->x + 1, pos->y).solid) {
              context.getCommandBuffer().patchComponent<Position>(
                  event.entityId, &Position::x, pos->x + 1);
            } else {
              context.getEventBus().publish(std::make_any<WorldCollisionEvent>(
                  WorldCollisionEvent{event.entityId}));
            }
          }
          break;
        }
      });
}

void initActorEvents(ECSContext &ecsContext) {
  subscribeMoveIntentEventListener(ecsContext);
}
