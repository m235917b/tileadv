#include <any>
#include <iostream>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#include "api.hpp"

struct Inventory {
  std::vector<std::string> inventory;
};

struct Equipment {
  std::string weapon;
};

struct Vitals {
  int health;
};

struct TestPayload {
  std::string text;
};

int main() {
  GameAPI api{};

  api.registerEntityEffect(
      "equip_item", [](ECSAPI &ecsApi, const std::any &payload) {
        // ecsApi.patchComponent(target, &Equipment::weapon, source);
        ecsApi.print(std::any_cast<TestPayload>(payload).text);
      });

  api.registerEventEffectTrigger<KeyDownEvent>(
      "equip_item",
      [](const KeyDownEvent &event) { return event.keycode == SDLK_K; },
      [](const KeyDownEvent &) {
        return std::make_any<TestPayload>(TestPayload{"fireball"});
      });

  api.run();

  return 0;
}
