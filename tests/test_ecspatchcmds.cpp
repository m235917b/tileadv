#include "ecs/ecscontext.hpp"
#include "thirdparty/catch_amalgamated.hpp"

struct TestResource {
  int val1;
  int val2;
};

struct TestComponent {
  int val1;
  int val2;
};

TEST_CASE("ECS patch resource works on single field") {
  ECSContext ctx{};

  ctx.getCommandBuffer().upsertResource<TestResource>(TestResource{7, 8});

  ctx.getCommandBuffer().flush();

  const auto &res{ctx.getResourceManager().getResource<TestResource>()};

  REQUIRE(res);
  REQUIRE(res->val1 == 7);
  REQUIRE(res->val2 == 8);

  ctx.getCommandBuffer().patchResource<TestResource, int>(&TestResource::val2,
                                                          10);

  ctx.getCommandBuffer().flush();

  REQUIRE(res);
  REQUIRE(res->val1 == 7);
  REQUIRE(res->val2 == 10);
}

TEST_CASE("ECS patch resource works with multiple patches") {
  ECSContext ctx{};

  ctx.getCommandBuffer().upsertResource<TestResource>(TestResource{7, 8});

  ctx.getCommandBuffer().flush();

  const auto &res{ctx.getResourceManager().getResource<TestResource>()};

  REQUIRE(res);
  REQUIRE(res->val1 == 7);
  REQUIRE(res->val2 == 8);

  ctx.getCommandBuffer().patchResource<TestResource, int>(&TestResource::val2,
                                                          10);
  ctx.getCommandBuffer().patchResource<TestResource, int>(&TestResource::val1,
                                                          -2);
  ctx.getCommandBuffer().patchResource<TestResource, int>(&TestResource::val2,
                                                          11);

  ctx.getCommandBuffer().flush();

  REQUIRE(res);
  REQUIRE(res->val1 == -2);
  REQUIRE(res->val2 == 11);
}

TEST_CASE("ECS patch component works on single field") {
  ECSContext ctx{};

  ctx.getCommandBuffer().upsertComponent<TestComponent>("comp1",
                                                        TestComponent{1, 2});
  ctx.getCommandBuffer().upsertComponent<TestComponent>("comp2",
                                                        TestComponent{7, 8});

  ctx.getCommandBuffer().flush();

  const auto &comp1{ctx.getStore().getComponent<TestComponent>("comp1")};
  const auto &comp2{ctx.getStore().getComponent<TestComponent>("comp2")};

  REQUIRE(comp1);
  REQUIRE(comp2);
  REQUIRE(comp1->val1 == 1);
  REQUIRE(comp1->val2 == 2);
  REQUIRE(comp2->val1 == 7);
  REQUIRE(comp2->val2 == 8);

  ctx.getCommandBuffer().patchComponent<TestComponent, int>(
      "comp1", &TestComponent::val2, 10);
  ctx.getCommandBuffer().patchComponent<TestComponent, int>(
      "comp2", &TestComponent::val1, 11);

  ctx.getCommandBuffer().flush();

  REQUIRE(comp1);
  REQUIRE(comp2);
  REQUIRE(comp1->val1 == 1);
  REQUIRE(comp1->val2 == 10);
  REQUIRE(comp2->val1 == 11);
  REQUIRE(comp2->val2 == 8);
}

TEST_CASE("ECS patch component works with multiple patches") {
  ECSContext ctx{};

  ctx.getCommandBuffer().upsertComponent<TestComponent>("comp1",
                                                        TestComponent{1, 2});
  ctx.getCommandBuffer().upsertComponent<TestComponent>("comp2",
                                                        TestComponent{7, 8});

  ctx.getCommandBuffer().flush();

  const auto &comp1{ctx.getStore().getComponent<TestComponent>("comp1")};
  const auto &comp2{ctx.getStore().getComponent<TestComponent>("comp2")};

  REQUIRE(comp1);
  REQUIRE(comp2);
  REQUIRE(comp1->val1 == 1);
  REQUIRE(comp1->val2 == 2);
  REQUIRE(comp2->val1 == 7);
  REQUIRE(comp2->val2 == 8);

  ctx.getCommandBuffer().patchComponent<TestComponent, int>(
      "comp1", &TestComponent::val2, 10);
  ctx.getCommandBuffer().patchComponent<TestComponent, int>(
      "comp2", &TestComponent::val1, 11);
  ctx.getCommandBuffer().patchComponent<TestComponent, int>(
      "comp1", &TestComponent::val1, -2);
  ctx.getCommandBuffer().patchComponent<TestComponent, int>(
      "comp2", &TestComponent::val1, 12);

  ctx.getCommandBuffer().flush();

  REQUIRE(comp1);
  REQUIRE(comp2);
  REQUIRE(comp1->val1 == -2);
  REQUIRE(comp1->val2 == 10);
  REQUIRE(comp2->val1 == 12);
  REQUIRE(comp2->val2 == 8);
}
