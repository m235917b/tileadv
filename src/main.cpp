#include <iostream>

#include "asc.hpp"
#include "ecs/ecsapi.hpp"
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
  ECSAPI ecsApi{ecsContext};

  ecsApi.getPrefab().registerRecipe<Position>(Position{-1.0, -2.0});
  ecsApi.getPrefab().registerRecipe<Position, Velocity>(
      "npc", Position{-3.0, -4.0}, Velocity{7.0, 8.0});

  /*ecsApi.instantiateEntity("entity1", "npc")
      .set<float, Position>(&Position::x, 10.5f)
      .set<float, Velocity>(&Velocity::vx, 11.1f)
      .finish();
  ecsApi.createEntity("entity2")
      .set<float, Position>(&Position::y, 22.22f)
      .finish();*/
  /*ecsApi.instantiateEntity("entity1", "npc").finish();
  ecsApi.createEntity("entity2").add<Position>().finish();*/
  ecsApi.instantiateEntity("entity1", "npc")
      .set(&Position::x, 5.5f)
      .set(&Velocity::vx, 0.3f)
      .finish();
  ecsApi.createEntity("entity2")
      .add<Position>()
      .set(&Position::y, 0.03f)
      .finish();

  /*ecsContext.getCommandBuffer().enqueue(std::make_any<UpsertComponent>(
      UpsertComponent{"entity1", Position{0.0f, 0.0f}}));*/
  /*ecsContext.getCommandBuffer().enqueue(
      UpsertComponent{"entity1", Position{0.0f, 0.0f}});
  ecsContext.getCommandBuffer().enqueue(
      UpsertComponent{"entity1", Velocity{1.0f, 2.0f}});
  ecsContext.getCommandBuffer().enqueue(
      UpsertComponent{"entity2", Position{10.0f, 10.0f}});*/

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  ecsApi.addViewSystem<Position>(
      "UpdatePositions", "MoveSystem",
      [](ECSContext &, const float dt, const std::string &id,
         const Position &pos) {
        std::cout << "Positions View: " << id << " " << pos.x << ", " << pos.y
                  << ", " << dt << std::endl;
      });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem", [](ECSContext &context, float) {
        context.getStore().view<Position, Velocity>([](const std::string &id,
                                                       const Position &pos,
                                                       const Velocity &vel) {
          std::cout << "Positions + Velocities: " << id << " " << pos.x << ", "
                    << pos.y << ", " << vel.vx << ", " << vel.vy << std::endl;
        });
      });

  /*ecsContext.view<Position, Velocity>(
      [](const std::string &id, const Position &pos, const Velocity &vel) {
        std::cout << "Updated Positions:" << id << " " << pos.x << ", " << pos.y
                  << ", " << vel.vx << std::endl;
      });*/

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

  /*ecsContext.getScheduler().addSystem(
      "PrintIds", "PrintSystem",
      [](ECSContext &context, [[maybe_unused]] float dt) {
        context.view<Position>(
            []([[maybe_unused]] const std::string &id, const Position &pos)
     { std::cout << "Positions:" << id << " " << pos.x << ", " << pos.y
                        << std::endl;
            });
      });*/

  /*ecsContext.getEventBus().subscribe<SetPosEvent>(
      [](ECSContext &context, const SetPosEvent &event) {
        std::cout << "SetPosEvent:" << std::endl;
        context.view<Position>(
            [&event](const std::string &id, const Position &pos) {
              std::cout << id << ", " << event.x << ", " << pos.x << std::endl;
            });
      });*/

  ecsApi.subscribeViewListener<SetPosEvent, Position>(
      [](ECSContext &, const SetPosEvent &event, const std::string &id,
         const Position &pos) {
        std::cout << id << ", " << pos.x << ", " << pos.y << ", "
                  << event.entityId << std::endl;
      });

  ecsContext.getEventBus().subscribe<SetPosEvent>(
      [](ECSContext &context, const SetPosEvent &) {
        context.getCommandBuffer().enqueue<UpsertComponent>(
            UpsertComponent{"entity1", Position{100.0f, 200.0f}});
      });

  /*ecsContext.getEventBus().subscribe(
      std::type_index(typeid(SetPosEvent)),
      [](ECSContext &context, const std::any &event) {
        context.view<Position>(
            [&event](const std::string &id, const Position &pos) {
              std::cout << "SetPosEvent: " << id << ", "
                        << std::any_cast<SetPosEvent>(event).x << ", " << pos.x
                        << std::endl;
            });
      });*/

  ecsContext.getScheduler().bootstrap();

  ecsContext.getScheduler().update(0.0f);
  std::cout << "-----" << std::endl;
  ecsContext.getCommandBuffer().enqueue(PrintCommand{"Hello World!"});
  ecsContext.getEventBus().publish(SetPosEvent{"entity1", 100, 200});
  ecsContext.getScheduler().update(0.1f);
  std::cout << "-----" << std::endl;
  ecsContext.getScheduler().update(0.2f);
  return 0;
}
