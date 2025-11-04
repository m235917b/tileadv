#include "ecs/ecscontext.hpp"
#include "thirdparty/catch_amalgamated.hpp"

struct TestResource {
  int val;
};

TEST_CASE("ECS upsert adds and retrieves components") {
  ECSContext ctx{};

  ctx.getCommandBuffer().upsertResource<TestResource>(TestResource{7});

  ctx.getCommandBuffer().flush();

  const TestResource *res{ctx.getResourceManager().getResource<TestResource>()};

  REQUIRE(res);
  REQUIRE(res->val == 7);
}
