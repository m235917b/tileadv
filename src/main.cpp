#include <any>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#include "api.hpp"
#include "utils/math.hpp"

// TODO: Implement custom TileTypes / ActorTypes with a custom texture map

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
    const auto &mousePos{ctxApi.getMouseTile()};

    if (equipment->weapon != "fireball") {
      return;
    }

    ctxApi.instantiateEntity("fireprojectile")
        .add<Position>(*pos)
        .add<Fireprojectile>(
            {0, bresenham(pos->x, pos->y, mousePos.first, mousePos.second)})
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

  api.registerEntityEffect(
      "remove_fireprojectile", [](GameAPI &ctxApi, const std::any &payload) {
        const auto &id{std::any_cast<std::string>(payload)};
        ctxApi.upsertComponent<Garbage>(id, Garbage{true});
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

  api.registerEventEffectTrigger<MouseDownEvent>(
      "shoot", [](const MouseDownEvent &event) {
        return event.button == SDL_BUTTON_LEFT
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

          const auto &currpos{fp.path.at(fp.pos)};
          const auto &nextPos{fp.path.at(fp.pos + 1)};

          if (nextPos.first < currpos.first) {
            ctxApi.publishEvent<MoveIntentEvent>(
                MoveIntentEvent{entityId, Direction::LEFT});
          } else if (nextPos.first > currpos.first) {
            ctxApi.publishEvent<MoveIntentEvent>(
                MoveIntentEvent{entityId, Direction::RIGHT});
          }

          if (nextPos.second < currpos.second) {
            ctxApi.publishEvent<MoveIntentEvent>(
                MoveIntentEvent{entityId, Direction::UP});
          } else if (nextPos.second > currpos.second) {
            ctxApi.publishEvent<MoveIntentEvent>(
                MoveIntentEvent{entityId, Direction::DOWN});
          }

          ctxApi.upsertComponent<Fireprojectile>(entityId,
                                                 {fp.pos + 1, fp.path});
        }

        ctxApi.upsertComponent<SpeedControl>(entityId,
                                             {sc.speed, int(timeLeft)});
      });

  api.registerEventEffectTrigger<WorldCollisionEvent>(
      "remove_fireprojectile", [&api](const WorldCollisionEvent &event) {
        return api.hasComponent<Fireprojectile>(event.entityId)
                   ? std::make_optional(
                         std::make_any<std::string>(event.entityId))
                   : std::nullopt;
      });

  api.run();

  return 0;
}
