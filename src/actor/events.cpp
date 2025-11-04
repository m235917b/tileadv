#include "actor/events.hpp"

#include "actor/components.hpp"
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
          if (!getTileAt(*chunk, pos->x, pos->y - 1).solid) {
            context.getCommandBuffer().patchComponent<Position>(
                event.entityId, &Position::y, pos->y - 1);
          }
          break;
        case Direction::LEFT:
          if (!getTileAt(*chunk, pos->x - 1, pos->y).solid) {
            context.getCommandBuffer().patchComponent<Position>(
                event.entityId, &Position::x, pos->x - 1);
          }
          break;
        case Direction::DOWN:
          if (!getTileAt(*chunk, pos->x, pos->y + 1).solid) {
            context.getCommandBuffer().patchComponent<Position>(
                event.entityId, &Position::y, pos->y + 1);
          }
          break;
        case Direction::RIGHT:
          if (!getTileAt(*chunk, pos->x + 1, pos->y).solid) {
            context.getCommandBuffer().patchComponent<Position>(
                event.entityId, &Position::x, pos->x + 1);
          }
          break;
        }
      });
}

void initActorEvents(ECSContext &ecsContext) {
  subscribeMoveIntentEventListener(ecsContext);
}
