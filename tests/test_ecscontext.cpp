#include "ecs/ecscontext.hpp"
#include "thirdparty/catch_amalgamated.hpp"

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

struct SetValCommand {
  int val;
};

struct SetValEvent {
  int val;
};

TEST_CASE("ECS upsert adds and retrieves components") {
  ECSContext ctx{};

  ctx.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity1", Position{1.f, 2.f}});

  ctx.getCommandBuffer().flush();

  const Position *entity{ctx.getStore().getComponent<Position>("entity1")};

  REQUIRE(entity);
  REQUIRE(entity->x == Catch::Approx(1.f));
  REQUIRE(entity->y == Catch::Approx(2.f));
}

TEST_CASE("ECS view iterates over entities correctly") {
  ECSContext ctx{};

  ctx.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity1", Position{1.f, 2.f}});
  ctx.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity2", Position{5.f, 6.f}});
  ctx.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity2", Velocity{7.f, 8.f}});

  ctx.getCommandBuffer().flush();

  int found{0};
  ctx.getStore().view<Position>([&](auto id, auto &pos) {
    if (id == "entity1") {
      REQUIRE(pos.x == Catch::Approx(1.f));
      REQUIRE(pos.y == Catch::Approx(2.f));
    } else if (id == "entity2") {
      REQUIRE(pos.x == Catch::Approx(5.f));
      REQUIRE(pos.y == Catch::Approx(6.f));
    } else {
      REQUIRE(false);
    }
    found++;
  });
  REQUIRE(found == 2);

  ctx.getStore().view<Position, Velocity>([&](auto id, auto &pos, auto &vel) {
    if (id == "entity2") {
      REQUIRE(vel.vx == Catch::Approx(7.f));
      REQUIRE(vel.vy == Catch::Approx(8.f));
    } else {
      REQUIRE(false);
    }
    found++;
  });
  REQUIRE(found == 3);
}

TEST_CASE("ECS scheduler runs systems in different phases") {
  ECSContext ecsContext{};

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int testNum{0};

  ecsContext.getScheduler().addSystem(
      "PrintIds", "MoveSystem",
      [&](ECSContext &context, float) { testNum /= 2; });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem",
      [&](ECSContext &context, float) { testNum = 10; });

  REQUIRE(testNum == 0);

  ecsContext.getScheduler().update(0.0f);

  REQUIRE(testNum == 5);
}

TEST_CASE("ECS scheduler runs systems with view calls") {
  ECSContext ecsContext{};

  ecsContext.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity1", Position{1.f, 2.f}});
  ecsContext.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity2", Position{5.f, 6.f}});
  ecsContext.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity2", Velocity{7.f, 8.f}});

  ecsContext.getScheduler().bootstrap();

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int found{0};

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem", [&](ECSContext &context, float) {
        context.getStore().view<Position>(
            [&](const std::string &id, const Position &pos) {
              if (id == "entity1") {
                REQUIRE(pos.x == Catch::Approx(1.f));
                REQUIRE(pos.y == Catch::Approx(2.f));
              } else if (id == "entity2") {
                REQUIRE(pos.x == Catch::Approx(5.f));
                REQUIRE(pos.y == Catch::Approx(6.f));
              } else {
                REQUIRE(false);
              }
              found++;
            });
      });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem", [&](ECSContext &context, float) {
        context.getStore().view<Position, Velocity>([&](const std::string &id,
                                                        const Position &pos,
                                                        const Velocity &vel) {
          if (id == "entity2") {
            REQUIRE(vel.vx == Catch::Approx(7.f));
            REQUIRE(vel.vy == Catch::Approx(8.f));
          } else {
            REQUIRE(false);
          }
          found++;
        });
      });

  REQUIRE(found == 0);
  ecsContext.getScheduler().update(0.0);
  REQUIRE(found == 3);
}

TEST_CASE("ECS scheduler runs systems with view calls in multiple updates") {
  ECSContext ecsContext{};

  ecsContext.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity1", Position{1.f, 2.f}});
  ecsContext.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity2", Position{5.f, 6.f}});
  ecsContext.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity2", Velocity{7.f, 8.f}});

  ecsContext.getScheduler().bootstrap();

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int found{0};

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem", [&](ECSContext &context, float) {
        context.getStore().view<Position>(
            [&](const std::string &id, const Position &pos) {
              if (id == "entity1") {
                REQUIRE(pos.x == Catch::Approx(1.f));
                REQUIRE(pos.y == Catch::Approx(2.f));
              } else if (id == "entity2") {
                REQUIRE(pos.x == Catch::Approx(5.f));
                REQUIRE(pos.y == Catch::Approx(6.f));
              } else {
                REQUIRE(false);
              }
              found++;
            });
      });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem", [&](ECSContext &context, float) {
        context.getStore().view<Position, Velocity>([&](const std::string &id,
                                                        const Position &pos,
                                                        const Velocity &vel) {
          if (id == "entity2") {
            REQUIRE(vel.vx == Catch::Approx(7.f));
            REQUIRE(vel.vy == Catch::Approx(8.f));
          } else {
            REQUIRE(false);
          }
          found++;
        });
      });

  REQUIRE(found == 0);
  ecsContext.getScheduler().update(0.0);
  REQUIRE(found == 3);
  ecsContext.getScheduler().update(0.0);
  REQUIRE(found == 6);
}

TEST_CASE("ECS scheduler runs commands correctly at end of scheduler update") {
  ECSContext ecsContext{};

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int testNum{0};

  ecsContext.getScheduler().addSystem(
      "PrintIds", "MoveSystem",
      [&](ECSContext &context, float) { testNum /= 2; });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem",
      [&](ECSContext &context, float) { testNum = 10; });

  ecsContext.getCommandBuffer().registerHandler<SetValCommand>(
      [&](ECSContext &, SetValCommand command) {
        REQUIRE(testNum == 5);
        testNum -= command.val;
      });

  REQUIRE(testNum == 0);

  ecsContext.getCommandBuffer().enqueue<SetValCommand>(SetValCommand{2});
  ecsContext.getScheduler().update(0.0f);

  REQUIRE(testNum == 3);
}

TEST_CASE("ECS scheduler runs events correctly between pre and post systems") {
  ECSContext ecsContext{};

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int testNum{0};

  ecsContext.getScheduler().addSystem(
      "PrintIds", "MoveSystem",
      [&](ECSContext &context, float) { testNum -= 2; });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem",
      [&](ECSContext &context, float) { testNum = 10; });

  ecsContext.getEventBus().subscribe<SetValEvent>(
      [&](ECSContext &, const SetValEvent &command) {
        REQUIRE(testNum == 10);
        testNum /= command.val;
      });

  REQUIRE(testNum == 0);

  ecsContext.getEventBus().publish<SetValEvent>(SetValEvent{2});
  ecsContext.getScheduler().update(0.0f);

  REQUIRE(testNum == 3);
}

TEST_CASE(
    "ECS scheduler submits and executes event correctly from pre systems") {
  ECSContext ecsContext{};

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int testNum{0};

  ecsContext.getScheduler().addSystem("PrintIds", "MoveSystem",
                                      [&](ECSContext &context, float) {
                                        REQUIRE(testNum == 5);
                                        testNum -= 2;
                                      });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem", [&](ECSContext &context, float) {
        testNum = 10;
        context.getEventBus().publish<SetValEvent>(SetValEvent{2});
        REQUIRE(testNum == 10);
      });

  ecsContext.getEventBus().subscribe<SetValEvent>(
      [&](ECSContext &, const SetValEvent &command) {
        REQUIRE(testNum == 10);
        testNum /= command.val;
      });

  REQUIRE(testNum == 0);

  ecsContext.getScheduler().update(0.0f);

  REQUIRE(testNum == 3);
}

TEST_CASE(
    "ECS scheduler submits and executes command correctly from pre systems") {
  ECSContext ecsContext{};

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int testNum{0};

  ecsContext.getScheduler().addSystem("PrintIds", "MoveSystem",
                                      [&](ECSContext &context, float) {
                                        REQUIRE(testNum == 10);
                                        testNum /= 2;
                                      });

  ecsContext.getScheduler().addSystem(
      "UpdatePositions", "MoveSystem", [&](ECSContext &context, float) {
        testNum = 10;
        ecsContext.getCommandBuffer().enqueue<SetValCommand>(SetValCommand{2});
        REQUIRE(testNum == 10);
      });

  ecsContext.getCommandBuffer().registerHandler<SetValCommand>(
      [&](ECSContext &, SetValCommand command) {
        REQUIRE(testNum == 5);
        testNum -= command.val;
      });

  REQUIRE(testNum == 0);

  ecsContext.getScheduler().update(0.0f);

  REQUIRE(testNum == 3);
}
