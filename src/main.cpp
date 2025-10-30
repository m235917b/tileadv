#include <iostream>

#include "asc.hpp"
#include "ecs/ecscommandtypes.hpp"
#include "ecs/ecscontext.hpp"

struct Position {
  float x;
  float y;
};

struct Velocity {
  float vx;
  float vy;
};

struct SetPosEvent {
  std::string entityId;
  float x;
  float y;
};

int main() {
  // ApplicationStateController appStateController{};

  // appStateController.run();

  ECSContext ecsContext{};

  ecsContext.getCommandBuffer().enqueue(
      UpsertComponent{"entity1", Position{0.0f, 0.0f}});
  /*ecsContext.getCommandBuffer().enqueue(std::make_any<UpsertComponent>(
      UpsertComponent{"entity1", Position{0.0f, 0.0f}}));*/
  ecsContext.getCommandBuffer().enqueue(
      UpsertComponent{"entity1", Velocity{1.0f, 2.0f}});
  ecsContext.getCommandBuffer().enqueue(
      UpsertComponent{"entity2", Position{10.0f, 10.0f}});

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem",
      [](ECSContext &context, [[maybe_unused]] float dt) {
        context.view<Position, Velocity>(
            []([[maybe_unused]] const std::string &id, Position &pos,
               Velocity &vel) {
              pos.x += vel.vx;
              pos.y += vel.vy;

              std::cout << "Updated Positions:" << id << " " << pos.x << ", "
                        << pos.y << std::endl;
            });
      });

  /*ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem",
      [](ECSContext &context, [[maybe_unused]] float dt) {
        const std::vector<std::type_index> types{
            std::type_index(typeid(Position)),
            std::type_index(typeid(Velocity))};
        context.view(types, []([[maybe_unused]] const std::string &id,
                               std::vector<std::any *> &components) {
          std::any_cast<Position>(components.at(0))->x +=
              std::any_cast<Velocity>(components.at(1))->vx;
          std::any_cast<Position>(components.at(0))->y +=
              std::any_cast<Velocity>(components.at(1))->vy;

          std::cout << "Updated Positions:" << id << " "
                    << std::any_cast<Position>(components.at(0))->x << ", "
                    << std::any_cast<Position>(components.at(0))->y
                    << std::endl;
        });
      });*/

  ecsContext.getScheduler().addSystem(
      "PrintIds", "PrintSystem",
      [](ECSContext &context, [[maybe_unused]] float dt) {
        context.view<Position>(
            []([[maybe_unused]] const std::string &id, Position &pos) {
              std::cout << "Positions:" << id << " " << pos.x << ", " << pos.y
                        << std::endl;
            });
      });

  ecsContext.getEventBus().subscribe<SetPosEvent>(
      [](ECSContext &context, const SetPosEvent &event) {
        context.view<Position>([&event](const std::string &id, Position &pos) {
          if (id == event.entityId) {
            pos.x = event.x;
            pos.y = event.y;
          }
        });
      });

  /*ecsContext.getEventBus().subscribe(
      std::type_index(typeid(SetPosEvent)),
      [](ECSContext &context, const std::any &event) {
        context.view<Position>([&event](const std::string &id, Position &pos) {
          if (id == std::any_cast<SetPosEvent>(event).entityId) {
            pos.x = std::any_cast<SetPosEvent>(event).x;
            pos.y = std::any_cast<SetPosEvent>(event).y;
          }
        });
      });*/

  ecsContext.getScheduler().bootstrap();
  ecsContext.getScheduler().update(0.0f);
  std::cout << "-----" << std::endl;
  ecsContext.getCommandBuffer().enqueue(PrintCommand{"Hello World!"});
  /*ecsContext.getCommandBuffer().enqueue(
      std::make_any<PrintCommand>(PrintCommand{"Hello World!"}));*/
  ecsContext.getEventBus().publish(SetPosEvent{"entity1", 100, 200});
  /*ecsContext.getEventBus().publish(
      std::make_any<SetPosEvent>(SetPosEvent{"entity1", 100, 200}));*/
  ecsContext.getScheduler().update(0.1f);
  std::cout << "-----" << std::endl;
  ecsContext.getScheduler().update(0.2f);
  return 0;
}
