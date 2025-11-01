#include "ecs/ecscontext.hpp"
#include "thirdparty/catch_amalgamated.hpp"

struct Position {
  float x;
  float y;
};

TEST_CASE("ECS Upsert adds and retrieves components") {
  ECSContext ctx;

  ctx.getCommandBuffer().enqueue<UpsertComponent>(
      UpsertComponent{"entity1", Position{1.f, 2.f}});
  ctx.getScheduler().update(0.f); // flush commands if required

  bool found = false;
  ctx.getStore().view<Position>([&](auto id, auto &pos) {
    if (id == "entity1") {
      REQUIRE(pos.x == Catch::Approx(1.f));
      REQUIRE(pos.y == Catch::Approx(2.f));
      found = true;
    }
  });
  REQUIRE(found);
}
