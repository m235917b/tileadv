#include "ecs/ecscontext.hpp"
#include "thirdparty/catch_amalgamated.hpp"

TEST_CASE("ECS scheduler correctly enables/disables systems") {
  ECSContext ecsContext{};

  ecsContext.getScheduler().addPhasePre("UpdatePositions");
  ecsContext.getScheduler().addPhasePost("PrintIds");

  int testNum1{0};
  int testNum2{0};

  ecsContext.getScheduler().registerSystem(
      "PrintIds", "System1", [&](ECSContext &context, float) { testNum1++; });

  ecsContext.getScheduler().registerSystem(
      "UpdatePositions", "System2",
      [&](ECSContext &context, float) { testNum2++; });

  REQUIRE(testNum1 == 0);
  REQUIRE(testNum2 == 0);
  ecsContext.getScheduler().update(0.0f);
  REQUIRE(testNum1 == 1);
  REQUIRE(testNum2 == 1);
  ecsContext.getScheduler().disableSystem("System1");
  ecsContext.getScheduler().disableSystem("System2");
  ecsContext.getScheduler().update(0.0f);
  REQUIRE(testNum1 == 1);
  REQUIRE(testNum2 == 1);
  ecsContext.getScheduler().enableSystem("System1");
  ecsContext.getScheduler().enableSystem("System2");
  ecsContext.getScheduler().update(0.0f);
  REQUIRE(testNum1 == 2);
  REQUIRE(testNum2 == 2);
}