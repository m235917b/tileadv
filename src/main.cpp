#include <any>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#include "api.hpp"
#include "utils/math.hpp"

struct Inventory {
  std::vector<std::string> inventory;
};

struct Equipment {
  std::string weapon;
};

struct Vitals {
  int health;
};

struct EquipItem {
  std::string item;
};

struct Fireball {
  int damage;
};

struct Fireprojectile {
  int pos;
  std::vector<std::pair<int, int>> path;
};

int main() {
  GameAPI api{};

  api.upsertComponent<Inventory>("player",
                                 {{"fireball", "health_potion", "dagger"}});
  api.upsertComponent<Equipment>("player", {""});
  api.upsertComponent<Fireball>("fireball", {10});

  api.getPrefab().registerRecipe<Actor, Position, Fireprojectile, SpeedControl>(
      "fireprojectile", {ActorType::FIREBALL}, {1, 1},
      {0, std::vector<std::pair<int, int>>()}, {100, 0});

  api.registerEntityEffect(
      "equip_item", [](GameAPI &ctxApi, const std::any &payload) {
        auto inv{ctxApi.getComponent<Inventory>("player")->inventory};
        auto equ{ctxApi.getComponent<Equipment>("player")->weapon};

        if (!equ.empty()) {
          inv.push_back(equ);
        }

        auto itemToEquip{std::any_cast<EquipItem>(payload)};

        auto it{std::find_if(inv.begin(), inv.end(),
                             [&itemToEquip](const auto &item) {
                               return item == itemToEquip.item;
                             })};

        if (it == inv.end()) {
          return;
        }

        equ = *it;

        inv.erase(it);

        ctxApi.upsertComponent<Inventory>("player", {inv});
        ctxApi.upsertComponent<Equipment>("player", {equ});
      });

  api.registerEntityEffect("shoot", [](GameAPI &ctxApi, const std::any &) {
    const auto &equipment{ctxApi.getComponent<Equipment>("player")};
    const auto &pos{ctxApi.getComponent<Position>("player")};

    if (equipment->weapon != "fireball") {
      return;
    }

    ctxApi.instantiateEntity("fireprojectile")
        .add<Position>(*pos)
        .add<Fireprojectile>({0, bresenham(pos->x, pos->y, 50, 50)})
        .finish();
  });

  api.registerEntityEffect(
      "print_player", [](GameAPI &ctxApi, const std::any &) {
        auto inv{ctxApi.getComponent<Inventory>("player")->inventory};
        auto equ{ctxApi.getComponent<Equipment>("player")->weapon};

        ctxApi.print("----------");

        for (const auto &item : inv) {
          ctxApi.print(item);
        }

        ctxApi.print("Equipped:");
        ctxApi.print(equ);
        ctxApi.print("----------");
      });

  api.registerEventEffectTrigger<KeyDownEvent>(
      "equip_item", [](const KeyDownEvent &event) {
        return event.keycode == SDLK_K
                   ? std::make_optional(
                         std::make_any<EquipItem>(EquipItem{"fireball"}))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<KeyDownEvent>(
      "equip_item", [](const KeyDownEvent &event) {
        return event.keycode == SDLK_L
                   ? std::make_optional(
                         std::make_any<EquipItem>(EquipItem{"dagger"}))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<KeyDownEvent>(
      "print_player", [](const KeyDownEvent &event) {
        return event.keycode == SDLK_P
                   ? std::make_optional(std::make_any<void *>(nullptr))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<KeyDownEvent>(
      "shoot", [](const KeyDownEvent &event) {
        return event.keycode == SDLK_SPACE
                   ? std::make_optional(std::make_any<void *>(nullptr))
                   : std::nullopt;
      });

  api.registerUpdate<Fireprojectile, SpeedControl>(
      [](GameAPI &ctxApi, float dt, const std::string &entityId,
         const Fireprojectile &fp, const SpeedControl &sc) {
        if (fp.pos >= int(fp.path.size()) - 1) {
          ctxApi.upsertComponent<Garbage>(entityId, {true});
          return;
        }

        auto timeLeft = sc.timeLeft - dt;

        if (timeLeft <= 0) {
          timeLeft = 1000 / sc.speed;

          const auto &nextPos{fp.path.at(fp.pos + 1)};

          ctxApi.upsertComponent<Fireprojectile>(entityId,
                                                 {fp.pos + 1, fp.path});
          ctxApi.upsertComponent<Position>(entityId,
                                           {nextPos.first, nextPos.second});
        }

        ctxApi.upsertComponent<SpeedControl>(entityId,
                                             {sc.speed, int(timeLeft)});
      });

  api.run();

  return 0;
}
