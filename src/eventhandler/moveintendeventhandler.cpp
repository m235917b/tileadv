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
        Position position{pos->x, pos->y};

        if (event.up > 0) {
          if (!getTileAt(*chunk, pos->x, pos->y - 1).solid) {
            position.y -= 1;
          }
        }
        if (event.left > 0) {
          if (!getTileAt(*chunk, pos->x - 1, pos->y).solid) {
            position.x -= 1;
          }
        }
        if (event.up < 0) {
          if (!getTileAt(*chunk, pos->x, pos->y + 1).solid) {
            position.y += 1;
          }
        }
        if (event.left < 0) {
          if (!getTileAt(*chunk, pos->x + 1, pos->y).solid) {
            position.x += 1;
          }
        }

        context.getCommandBuffer().upsertComponent(event.entityId, position);
      });
}
