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

  auto mainMenu{
      std::make_unique<GUIComponent>("main_menu", .4f, .3f, .2f, .4f)};
  mainMenu->setVisible(false);
  mainMenu->setLayout(GUILayout::VERTICAL);
  mainMenu->setBorder(true);
  mainMenu->setSpacing(.02f);
  mainMenu->setCenterLeft(true);
  mainMenu->setCenterTop(true);

  auto headline{std::make_unique<GUIComponent>("1_headline", 0, 0, .96f, .3f)};
  headline->setType(GUIElementType::ELEMENT);
  headline->setText("Main Menu");
  headline->setBorder(true);
  headline->setScale(.7f);
  headline->setCenterTop(true);
  headline->setCenterLeft(true);
  auto line1{std::make_unique<GUIComponent>("2_play", 0, 0, .96f, .3f)};
  line1->setType(GUIElementType::ELEMENT);
  line1->setText("Play Game");
  line1->setBorder(true);
  line1->setScale(.7f);
  line1->setCenterTop(true);
  line1->setCenterLeft(true);
  auto line2{std::make_unique<GUIComponent>("3_exit", 0, 0, .96f, .3f)};
  line2->setType(GUIElementType::ELEMENT);
  line2->setText("Exit Game");
  line2->setBorder(true);
  line2->setScale(.7f);
  line2->setCenterTop(true);
  line2->setCenterLeft(true);

  mainMenu->addChild(std::move(headline));
  mainMenu->addChild(std::move(line1));
  mainMenu->addChild(std::move(line2));

  api.getGUIContext().addComponent(std::move(mainMenu));
  api.getGUIContext().setComponentVisible("main_menu", true);

  api.getGUIContext().addMouseButtonListener(
      "2_play", SDL_BUTTON_LEFT, [&api]() {
        api.setApplicationState(ApplicationState::RUNNING);
        api.getGUIContext().setComponentVisible("main_menu", false);
      });

  api.getGUIContext().addMouseButtonListener(
      "3_exit", SDL_BUTTON_LEFT,
      [&api]() { api.setApplicationState(ApplicationState::QUIT); });

  api.registerEffect("toggle_main_menu", [](GameAPI &ctxApi, const std::any &) {
    const auto appState{std::any_cast<ApplicationStateResource>(
        ctxApi.getResource(std::type_index(typeid(ApplicationStateResource))))};
    if (appState->state == ApplicationState::MAIN_MENU) {
      ctxApi.setApplicationState(ApplicationState::RUNNING);
      ctxApi.getGUIContext().setComponentVisible("main_menu", false);
    } else if (appState->state == ApplicationState::RUNNING) {
      ctxApi.setApplicationState(ApplicationState::MAIN_MENU);
      ctxApi.getGUIContext().setComponentVisible("main_menu", true);
    }
  });

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

  api.getPrefab().registerRecipe<Actor, Position, Fireprojectile, SpeedControl>(
      "fireprojectile", {ActorType::FIREBALL}, {1, 1},
      {0, std::vector<std::pair<int, int>>()}, {100, 0}, Physics{true});

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
        .add<Fireprojectile>(
            {0, bresenham(pos->x, pos->y, mousePos.first, mousePos.second)})
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
      "fireprojectile_hit", [](GameAPI &ctxApi, const std::any &payload) {
        const auto &entities{
            std::any_cast<std::pair<std::string, std::string>>(payload)};
        ctxApi.upsertComponent<Garbage>(entities.first, Garbage{true});
        ctxApi.upsertComponent<Garbage>(entities.second, Garbage{true});
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

  api.registerEventEffectTrigger<ActorCollisionEvent>(
      "fireprojectile_hit", [&api](const ActorCollisionEvent &event) {
        return api.hasComponent<Fireprojectile>(event.moved)
                   ? std::make_optional(
                         std::make_any<std::pair<std::string, std::string>>(
                             event.moved, event.other))
                   : std::nullopt;
      });

  api.run();

  return 0;
}
