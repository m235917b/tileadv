#include "eventhandler/moveintenteventhandler.hpp"

#include <any>

#include "components/position.hpp"
#include "ecs/ecs.hpp"
#include "events/moveIntentEvent.hpp"
#include "resources/chunkresource.hpp"

void subscribeMoveIntentEventHandler(ECSContext &ecsContext) {
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
