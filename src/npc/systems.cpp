#include "npc/systems.hpp"

#include "actor/components.hpp"
#include "asc/resources.hpp"

const auto spawnDummySystem{[](ECSContext &ecsContext, const float) {
  auto &ecsApi{
      *ecsContext.getResourceManager().getResource<ECSAPIResource>()->ecsApi};
  ecsApi.instantiateEntity("dummy1", "dummy")
      .set(&Position::x, 80)
      .set(&Position::y, 20)
      .finish();
  ecsApi.instantiateEntity("dummy2", "dummy")
      .set(&Position::x, 50)
      .set(&Position::y, 25)
      .finish();
  ecsApi.instantiateEntity("dummy3", "dummy")
      .set(&Position::x, 30)
      .set(&Position::y, 50)
      .finish();
}};

std::vector<SystemRegEntry> getNPCSystems() {
  return {SystemRegEntry{"dummy_spawn", spawnDummySystem}};
}
