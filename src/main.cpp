#include <any>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#include "guisetup.hpp"
#include "utils/math.hpp"

// TODO: Implement custom TileTypes / ActorTypes with a custom texture map
// TODO: Expose init OneShotPhase so the user doesn't have to meta-init
// TODO: Rework spawning system
// TODO: Add hit test for mouse position -> entity / component

struct Inventory {
  std::vector<std::string> inventory;
};

struct Equipment {
  std::string weapon;
};

struct EquipItem {
  std::string item;
};

struct Fireprojectile {
  int damage;
  int pos;
  std::vector<std::pair<int, int>> path;
};

struct FireTimer {
  float reset;
  float timeLeft;
};

struct EnemyFireEvent {
  int posX;
  int posY;
};

int main() {
  GameAPI api{};

  setupGUI(api);

  api.registerEventEffectTrigger<KeyDownEvent>(
      "toggle_main_menu", [](const KeyDownEvent &event) {
        return event.keycode == SDLK_ESCAPE
                   ? std::make_optional(std::make_any<bool>(true))
                   : std::nullopt;
      });

  api.setApplicationState(ApplicationState::MAIN_MENU);

  api.upsertComponent<Inventory>("player",
                                 {{"fireball", "health_potion", "dagger"}});
  api.upsertComponent<Equipment>("player", {""});
  api.upsertComponent<Vitals>("player", {100});

  api.getPrefab()
      .registerRecipe<Actor, Position, Fireprojectile, SpeedControl, Physics>(
          "fireprojectile", {ActorType::FIREBALL}, {1, 1},
          {10, 0, std::vector<std::pair<int, int>>()}, {100, 0}, Physics{true});

  api.getPrefab().registerRecipe<Actor, Position, Vitals, Physics, FireTimer>(
      "training_dummy", {ActorType::DUMMY}, {1, 1}, {100}, {true}, {1, 1});

  api.instantiateEntity("training_dummy")
      .set(&Position::x, 60)
      .set(&Position::y, 20)
      .finish();
  api.instantiateEntity("training_dummy")
      .set(&Position::x, 60)
      .set(&Position::y, 25)
      .finish();
  api.instantiateEntity("training_dummy")
      .set(&Position::x, 60)
      .set(&Position::y, 30)
      .finish();

  api.registerEffect(
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

  api.registerEffect("shoot", [](GameAPI &ctxApi, const std::any &) {
    const auto &equipment{ctxApi.getComponent<Equipment>("player")};
    const auto &pos{ctxApi.getComponent<Position>("player")};
    const auto &mousePos{ctxApi.getMouseTile()};

    if (equipment->weapon != "fireball") {
      return;
    }

    ctxApi.instantiateEntity("fireprojectile")
        .add<Position>(*pos)
        .set(&Fireprojectile::path,
             bresenham(pos->x, pos->y, mousePos.first, mousePos.second))
        .finish();
  });

  api.registerEffect("print_player", [](GameAPI &ctxApi, const std::any &) {
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

  api.registerEffect("remove_fireprojectile",
                     [](GameAPI &ctxApi, const std::any &payload) {
                       const auto &id{std::any_cast<std::string>(payload)};
                       ctxApi.upsertComponent<Garbage>(id, Garbage{true});
                     });

  api.registerEffect(
      "enemy_fire", [](GameAPI &ctxApi, const std::any &payload) {
        const auto &pos{std::any_cast<std::pair<int, int>>(payload)};
        for (float alpha{0.f}; alpha < 2.f * std::numbers::pi; alpha += 0.1f) {
          const int startX{int(pos.first + 10.f * std::cos(alpha))};
          const int startY{int(pos.second + 10.f * std::sin(alpha))};
          const int targetX{int(pos.first + 1000.f * std::cos(alpha))};
          const int targetY{int(pos.second + 1000.f * std::sin(alpha))};
          ctxApi.instantiateEntity("fireprojectile")
              .add<Position>({startX, startY})
              .set(&Fireprojectile::path,
                   bresenham(startX, startY, targetX, targetY))
              .finish();
        }
      });

  api.registerEffect("fireprojectile_hit", [](GameAPI &ctxApi,
                                              const std::any &payload) {
    const auto &entities{
        std::any_cast<std::pair<std::string, std::string>>(payload)};
    if (ctxApi.hasComponent(std::type_index(typeid(Vitals)), entities.second) &&
        ctxApi.hasComponent(std::type_index(typeid(Fireprojectile)),
                            entities.first)) {
      const auto damage{
          std::any_cast<Fireprojectile>(
              ctxApi.getComponent(entities.first,
                                  std::type_index(typeid(Fireprojectile))))
              ->damage};
      const auto health{
          std::any_cast<Vitals>(
              ctxApi.getComponent(entities.second,
                                  std::type_index(typeid(Vitals))))
              ->health};
      if (health - damage > 0) {
        ctxApi.upsertComponent(entities.second,
                               std::make_any<Vitals>(Vitals{health - damage}));
      } else {
        ctxApi.upsertComponent<Garbage>(entities.second, Garbage{true});
      }
      ctxApi.upsertComponent<Garbage>(entities.first, Garbage{true});
    } else {
      ctxApi.upsertComponent<Garbage>(entities.first, Garbage{true});
      ctxApi.upsertComponent<Garbage>(entities.second, Garbage{true});
    }
  });

  api.registerEventEffectTrigger<GameKeyDownEvent>(
      "equip_item", [](const GameKeyDownEvent &event) {
        return event.keycode == SDLK_K
                   ? std::make_optional(
                         std::make_any<EquipItem>(EquipItem{"fireball"}))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<GameKeyDownEvent>(
      "equip_item", [](const GameKeyDownEvent &event) {
        return event.keycode == SDLK_L
                   ? std::make_optional(
                         std::make_any<EquipItem>(EquipItem{"dagger"}))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<GameKeyDownEvent>(
      "print_player", [](const GameKeyDownEvent &event) {
        return event.keycode == SDLK_P
                   ? std::make_optional(std::make_any<void *>(nullptr))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<GameMouseDownEvent>(
      "shoot", [](const GameMouseDownEvent &event) {
        return event.button == SDL_BUTTON_LEFT
                   ? std::make_optional(std::make_any<void *>(nullptr))
                   : std::nullopt;
      });

  /*api.registerEventEffectTrigger<GameKeyDownEvent>(
      "shoot", [&ctr, &api](const GameKeyDownEvent &event) {
        ctr++;
        api.print(std::to_string(ctr));
        return event.keycode == SDLK_SPACE
                   ? std::make_optional(std::make_any<void *>(nullptr))
                   : std::nullopt;
      });*/

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

          ctxApi.upsertComponent<Fireprojectile>(
              entityId, {fp.damage, fp.pos + 1, fp.path});
        }

        ctxApi.upsertComponent<SpeedControl>(entityId,
                                             {sc.speed, int(timeLeft)});
      });

  api.registerUpdate<Position>([](GameAPI &ctxApi, float,
                                  const std::string &entityId,
                                  const Position &pos) {
    const auto mouse{std::any_cast<MousePosResource>(
        ctxApi.getResource(std::type_index(typeid(MousePosResource))))};

    if (pos.x == mouse->tileX && pos.y == mouse->tileY) {
      ctxApi.upsertResource(std::make_any<Focussed>(Focussed{entityId}));
    }
  });

  api.registerUpdate<FireTimer>([](GameAPI &ctxApi, float dt,
                                   const std::string &entityId,
                                   const FireTimer &timer) {
    auto newTime{timer.timeLeft - dt};

    if (newTime < 0) {
      newTime = timer.reset;
      const auto pos{ctxApi.getComponent<Position>(entityId)};
      ctxApi.publishEvent<EnemyFireEvent>(EnemyFireEvent{pos->x, pos->y});
    }

    ctxApi.upsertComponent<FireTimer>(entityId,
                                      FireTimer{timer.reset, newTime});
  });

  api.registerUpdate<Vitals>([](GameAPI &ctxApi, float,
                                const std::string &entityId,
                                const Vitals &vitals) {
    if (entityId == "player" && vitals.health <= 0) {
      ctxApi.setApplicationState(ApplicationState::QUIT);
    }
  });

  api.registerEventEffectTrigger<WorldCollisionEvent>(
      "remove_fireprojectile", [&api](const WorldCollisionEvent &event) {
        return api.hasComponent<Fireprojectile>(event.entityId)
                   ? std::make_optional(
                         std::make_any<std::string>(event.entityId))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<ActorCollisionEvent>(
      "fireprojectile_hit", [&api](const ActorCollisionEvent &event) {
        return api.hasComponent<Fireprojectile>(event.moved)
                   ? std::make_optional(
                         std::make_any<std::pair<std::string, std::string>>(
                             event.moved, event.other))
                   : std::nullopt;
      });

  api.registerEventEffectTrigger<EnemyFireEvent>(
      "enemy_fire", [](const EnemyFireEvent &event) {
        return std::make_optional(
            std::make_any<std::pair<int, int>>(event.posX, event.posY));
      });

  api.run();

  return 0;
}
