#include "ecs/ecsapi.hpp"
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

TEST_CASE("ECS entity builder works") {
  ECSContext ctx{};
  ECSAPI ecsApi{ctx};

  ecsApi.createEntity("entity1")
      .add(Position{2.f, 3.f})
      .set(&Position::x, -1.f)
      .finish();

  ctx.getScheduler().bootstrap();

  const Position *entity{ctx.getStore().getComponent<Position>("entity1")};

  REQUIRE(entity);
  REQUIRE(entity->x == Catch::Approx(-1.f));
  REQUIRE(entity->y == Catch::Approx(3.f));
}

TEST_CASE("ECS entity with component prefab works") {
  ECSContext ctx{};
  ECSAPI ecsApi{ctx};

  ecsApi.getPrefab().registerRecipe<Position>(Position{-1.f, -2.f});

  ecsApi.createEntity("entity1")
      .add<Position>()
      .set(&Position::x, -3.f)
      .finish();

  ecsApi.createEntity("entity2").set(&Position::x, -4.f).finish();

  ctx.getScheduler().bootstrap();

  const Position *entity{ctx.getStore().getComponent<Position>("entity1")};
  const Position *entity2{ctx.getStore().getComponent<Position>("entity2")};

  REQUIRE(entity);
  REQUIRE(entity->x == Catch::Approx(-3.f));
  REQUIRE(entity->y == Catch::Approx(-2.f));

  REQUIRE(entity2);
  REQUIRE(entity2->x == Catch::Approx(-4.f));
  REQUIRE(entity2->y == Catch::Approx(-2.f));
}

TEST_CASE("ECS entity with entity prefab works") {
  ECSContext ctx{};
  ECSAPI ecsApi{ctx};

  ecsApi.getPrefab().registerRecipe("npc", Position{-1.f, -2.f},
                                    Velocity{1.f, 2.f});

  ecsApi.instantiateEntity("entity1", "npc")
      .set(&Position::x, -3.f)
      .set(&Velocity::vx, -6.f)
      .finish();

  ctx.getScheduler().bootstrap();

  const Position *entity{ctx.getStore().getComponent<Position>("entity1")};
  const Velocity *entityVel{ctx.getStore().getComponent<Velocity>("entity1")};

  REQUIRE(entity);
  REQUIRE(entity->x == Catch::Approx(-3.f));
  REQUIRE(entity->y == Catch::Approx(-2.f));

  REQUIRE(entityVel);
  REQUIRE(entityVel->vx == Catch::Approx(-6.f));
  REQUIRE(entityVel->vy == Catch::Approx(2.f));
}

TEST_CASE("ECS scheduler runs view systems") {
  ECSContext ecsContext{};
  ECSAPI ecsApi{ecsContext};

  ecsApi.getPrefab().registerRecipe<Position>(Position{1.f, 2.f});
  ecsApi.getPrefab().registerRecipe("npc", Position{5.f, 6.f},
                                    Velocity{7.f, 8.f});

  ecsApi.createEntity("entity1").add<Position>().finish();
  ecsApi.instantiateEntity("entity2", "npc").finish();

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
