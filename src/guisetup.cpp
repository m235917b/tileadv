#include "guisetup.hpp"

#include <any>
#include <string>
#include <typeindex>

void setupGUI(GameAPI &api) {
  api.upsertResource(std::make_any<Focussed>(Focussed{""}));

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
  // api.getGUIContext().setComponentVisible("main_menu", true);

  api.getGUIContext().addMouseButtonListener(
      "2_play", SDL_BUTTON_LEFT, [&api]() {
        api.setApplicationState(ApplicationState::RUNNING);
        api.getGUIContext().setComponentVisible("main_menu", false);
      });

  api.getGUIContext().addMouseButtonListener(
      "3_exit", SDL_BUTTON_LEFT,
      [&api]() { api.setApplicationState(ApplicationState::QUIT); });

  api.getGUIContext().addKeyListener("2_play", SDLK_RETURN, [&api]() {
    api.setApplicationState(ApplicationState::RUNNING);
    api.getGUIContext().setComponentVisible("main_menu", false);
  });

  api.getGUIContext().addKeyListener("3_exit", SDLK_RETURN, [&api]() {
    api.setApplicationState(ApplicationState::QUIT);
  });

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

  // ------------------------------------------------------------------------------

  auto characterMenu{
      std::make_unique<GUIComponent>("char_menu", 0.f, 0.f, 1.f, 1.f)};
  characterMenu->setVisible(false);

  auto characterMenuContainer{std::make_unique<GUIComponent>(
      "char_menu_container", .05f, .1f, .4f, .3f)};
  characterMenuContainer->setLayout(GUILayout::VERTICAL);
  characterMenuContainer->setBorder(true);
  characterMenuContainer->setBackground(true);

  auto characterMenuContainer2{std::make_unique<GUIComponent>(
      "char_menu_container_2", .55f, .1f, .4f, .3f)};
  characterMenuContainer2->setImage("guiassets/border_brown.png");
  characterMenuContainer2->setFittingMode(GUIFittingMode::SCALE);
  characterMenuContainer2->setBackground(true);
  characterMenuContainer2->setBgColor(0x946246FF);

  auto headline11{
      std::make_unique<GUIComponent>("char_headline", 0.f, 0.f, 1.f, .2f)};
  headline11->setType(GUIElementType::ELEMENT);
  headline11->setText("Character Menu");
  headline11->setScale(3.f);
  headline11->setFittingMode(GUIFittingMode::CLIP);

  auto headline22{
      std::make_unique<GUIComponent>("char_headline_22", 0.f, 0.f, 1.f, .2f)};
  headline22->setType(GUIElementType::ELEMENT);
  headline22->setText("Inventory");
  headline22->setScale(4.f);
  headline22->setFittingMode(GUIFittingMode::SCALE);

  characterMenuContainer->addChild(std::move(headline11));
  characterMenuContainer->addChild(std::move(headline22));

  auto headline2{
      std::make_unique<GUIComponent>("char_headline_2", .02f, .02f, .5f, .5f)};
  headline2->setType(GUIElementType::ELEMENT);
  headline2->setImage("guiassets/test_button.png");

  characterMenuContainer2->addChild(std::move(headline2));

  auto characterMenuContainer3{std::make_unique<GUIComponent>(
      "char_menu_container_3", .3f, .5f, .4f, .4f)};
  characterMenuContainer3->setLayout(GUILayout::VERTICAL);
  characterMenuContainer3->setImage("guiassets/border_brown.png");
  characterMenuContainer3->setFittingMode(GUIFittingMode::SCALE);
  characterMenuContainer3->setCenterTop(true);
  characterMenuContainer3->setCenterLeft(true);

  auto subcontainer1{
      std::make_unique<GUIComponent>("sub_container_1", 0.f, 0.f, .5f, .4f)};
  subcontainer1->setLayout(GUILayout::VERTICAL);
  subcontainer1->setImage("guiassets/border_brown.png");
  subcontainer1->setFittingMode(GUIFittingMode::SCALE);
  subcontainer1->setSpacing(.02f);

  auto line11{std::make_unique<GUIComponent>("line_1", .02f, 0.f, .98f, .3f)};
  line11->setType(GUIElementType::ELEMENT);
  line11->setText("(0, 0)");
  line11->setScale(.8f);
  line11->setUpdateListener([&api](GUIComponent &component) {
    const auto pos{std::any_cast<Position>(
        api.getComponent("player", std::type_index(typeid(Position))))};
    component.setText("(" + std::to_string(pos->x) + ", " +
                      std::to_string(pos->y) + ")");
  });

  auto line22{std::make_unique<GUIComponent>("line_2", .02f, 0.f, .98f, .3f)};
  line22->setType(GUIElementType::ELEMENT);
  line22->setText("Line 2");
  line22->setScale(.8f);

  subcontainer1->addChild(std::move(line11));
  subcontainer1->addChild(std::move(line22));

  auto subcontainer2{
      std::make_unique<GUIComponent>("sub_container_2", .1f, 0, .8f, .96f)};
  subcontainer2->setLayout(GUILayout::VERTICAL);
  subcontainer2->setImage("guiassets/border_brown.png");
  subcontainer2->setFittingMode(GUIFittingMode::SCALE);
  subcontainer2->setSpacing(.02f);

  auto line3{std::make_unique<GUIComponent>("line_3", .02f, .02f, .98f, .3f)};
  line3->setType(GUIElementType::ELEMENT);
  line3->setText("Line 3");
  line3->setScale(.8f);

  auto line4{std::make_unique<GUIComponent>("line_4", .02f, .02f, .98f, .3f)};
  line4->setType(GUIElementType::ELEMENT);
  line4->setText("Line 4");
  line4->setScale(.8f);

  subcontainer2->addChild(std::move(line3));
  subcontainer2->addChild(std::move(line4));

  auto subcontainer3{
      std::make_unique<GUIComponent>("sub_container_3", 0.f, 0.f, .8f, .4f)};
  subcontainer3->setLayout(GUILayout::VERTICAL);
  subcontainer3->setImage("guiassets/border_brown.png");
  subcontainer3->setFittingMode(GUIFittingMode::SCALE);
  subcontainer3->setSpacing(.02);

  subcontainer3->addChild(std::move(subcontainer2));

  characterMenuContainer3->addChild(std::move(subcontainer1));
  characterMenuContainer3->addChild(std::move(subcontainer3));

  characterMenu->addChild(std::move(characterMenuContainer));
  characterMenu->addChild(std::move(characterMenuContainer2));
  characterMenu->addChild(std::move(characterMenuContainer3));

  api.getGUIContext().addComponent(std::move(characterMenu));

  auto statusBar{
      std::make_unique<GUIComponent>("statusbar", 0.f, .97f, 1.f, .03f)};
  statusBar->setLayout(GUILayout::HORIZONTAL);
  statusBar->setCenterTop(true);
  statusBar->setCenterLeft(true);
  statusBar->setFittingMode(GUIFittingMode::CLIP);
  statusBar->setNavigable(false);
  auto status1{std::make_unique<GUIComponent>("1_status", 0.f, 0.f, .5f, 1.f)};
  status1->setText("Status 1");
  status1->setScale(.5f);
  status1->setCenterLeft(true);
  status1->setCenterTop(true);
  status1->setFittingMode(GUIFittingMode::CLIP);
  status1->setNavigable(false);
  status1->setUpdateListener([&api](GUIComponent &component) {
    const auto focussed{std::any_cast<Focussed>(
        api.getResource(std::type_index(typeid(Focussed))))};
    if (focussed) {
      const auto vit{std::any_cast<Vitals>(
          api.getComponent(focussed->id, std::type_index(typeid(Vitals))))};
      if (vit) {
        component.setText("Health: " + std::to_string(vit->health));
      }
    }
  });
  statusBar->addChild(std::move(status1));
  auto status2{std::make_unique<GUIComponent>("2_status", 0.f, 0.f, .5f, 1.f)};
  status2->setUpdateListener([&api](GUIComponent &component) {
    const auto pos{std::any_cast<Position>(
        api.getComponent("player", std::type_index(typeid(Position))))};
    component.setText("Position: (" + std::to_string(pos->x) + ", " +
                      std::to_string(pos->y) + ")");
  });
  status2->setScale(.5f);
  status2->setCenterLeft(true);
  status2->setCenterTop(true);
  status2->setFittingMode(GUIFittingMode::CLIP);
  status2->setNavigable(false);
  statusBar->addChild(std::move(status2));
  api.getGUIContext().addComponent(std::move(statusBar));
  api.getGUIContext().setComponentVisible("statusbar");
}