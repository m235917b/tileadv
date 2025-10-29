#include <iostream>

#include "asc.hpp"
#include "ecs/ecscontext.hpp"

struct Position {
  float x;
  float y;
};

struct Velocity {
  float vx;
  float vy;
};

int main() {
  // ApplicationStateController appStateController{};

  // appStateController.run();

  ECSContext ecsContext{};

  ecsContext.getStore().createEntity("entity1");
  ecsContext.getStore().addComponent("entity1", Position{0.0f, 0.0f});
  ecsContext.getStore().addComponent("entity1", Velocity{1.0f, 2.0f});
  ecsContext.getStore().createEntity("entity2");
  ecsContext.getStore().addComponent("entity2", Position{10.0f, 10.0f});

  ecsContext.getScheduler().addPhase("UpdatePositions");
  ecsContext.getScheduler().addPhase("PrintIds");

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem",
      [](ECSContext &context, [[maybe_unused]] float dt) {
        context.getStore().view<Position, Velocity>(
            []([[maybe_unused]] const std::string &id, Position &pos,
               Velocity &vel) {
              pos.x += vel.vx;
              pos.y += vel.vy;

              std::cout << "Updated Positions:" << id << " " << pos.x << ", "
                        << pos.y << std::endl;
            });
      });

  ecsContext.getScheduler().addSystem(
      "PrintIds", "PrintSystem",
      [](ECSContext &context, [[maybe_unused]] float dt) {
        context.getStore().view<Position>(
            []([[maybe_unused]] const std::string &id, Position &pos) {
              std::cout << "Updated Positions:" << id << " " << pos.x << ", "
                        << pos.y << std::endl;
            });
      });

  ecsContext.getScheduler().update(0.0f);
  ecsContext.getScheduler().update(0.1f);
  ecsContext.getScheduler().update(0.2f);
  return 0;
}
