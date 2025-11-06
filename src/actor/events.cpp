#include "actor/events.hpp"

#include <typeindex>

#include "actor/resources.hpp"
#include "ecs/ecs.hpp"

void subscribeMoveIntentEventListener(ECSContext &ecsContext) {
  ecsContext.getEventBus().subscribe<MoveIntentEvent>(
      [](ECSContext &context, const MoveIntentEvent &event) {
        context.getCommandBuffer().patchResource(
            std::type_index(typeid(MoveIntentEventQueue)),
            [=](std::any &queueAny) {
              auto &queue{std::any_cast<MoveIntentEventQueue &>(queueAny)};
              queue.moveIntents.emplace_back(event);
            });
      });
}

void initActorEvents(ECSContext &ecsContext) {
  subscribeMoveIntentEventListener(ecsContext);
}
