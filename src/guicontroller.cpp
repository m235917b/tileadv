#include <SDL3/SDL.h>

#include "gui/guicomponent.hpp"
#include "guicontroller.hpp"
#include "utils/rendercontext.hpp"

#include "model/character.hpp"

GUIController::GUIController(const RenderContext &renderContext)
    : guiContext(renderContext) {
  // Initialize main menu elements
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

  guiContext.addComponent(std::move(mainMenu));
}

bool GUIController::init() { return guiContext.init(); }

void GUIController::initGameMenus(const Character &player) {
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

  auto headline{
      std::make_unique<GUIComponent>("char_headline", 0.f, 0.f, 1.f, .2f)};
  headline->setType(GUIElementType::ELEMENT);
  headline->setText("Character Menu");
  headline->setScale(3.f);
  headline->setFittingMode(GUIFittingMode::CLIP);

  auto headline22{
      std::make_unique<GUIComponent>("char_headline_22", 0.f, 0.f, 1.f, .2f)};
  headline22->setType(GUIElementType::ELEMENT);
  headline22->setText("Inventory");
  headline22->setScale(4.f);
  headline22->setFittingMode(GUIFittingMode::SCALE);

  characterMenuContainer->addChild(std::move(headline));
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

  auto line1{std::make_unique<GUIComponent>("line_1", .02f, 0.f, .98f, .3f)};
  line1->setType(GUIElementType::ELEMENT);
  line1->setText("(0, 0)");
  line1->setScale(.8f);
  line1->setUpdateListener([&player](GUIComponent &component) {
    component.setText("(" + std::to_string(player.getPosX()) + ", " +
                      std::to_string(player.getPosY()) + ")");
  });

  auto line2{std::make_unique<GUIComponent>("line_2", .02f, 0.f, .98f, .3f)};
  line2->setType(GUIElementType::ELEMENT);
  line2->setText("Line 2");
  line2->setScale(.8f);

  subcontainer1->addChild(std::move(line1));
  subcontainer1->addChild(std::move(line2));

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

  guiContext.addComponent(std::move(characterMenu));

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
  statusBar->addChild(std::move(status1));
  auto status2{std::make_unique<GUIComponent>("2_status", 0.f, 0.f, .5f, 1.f)};
  status2->setUpdateListener([&player](GUIComponent &component) {
    component.setText("Position: (" + std::to_string(player.getPosX()) + ", " +
                      std::to_string(player.getPosY()) + ")");
  });
  status2->setScale(.5f);
  status2->setCenterLeft(true);
  status2->setCenterTop(true);
  status2->setFittingMode(GUIFittingMode::CLIP);
  status2->setNavigable(false);
  statusBar->addChild(std::move(status2));
  guiContext.addComponent(std::move(statusBar));
  guiContext.setComponentVisible("statusbar");
}

void GUIController::keyDownListener(const SDL_Keycode key) {
  guiContext.keyDownListener(key);
}

void GUIController::mouseMotionListener(const float posX, const float posY) {
  guiContext.mouseMotionListener(posX, posY);
}

void GUIController::mouseButtonDownListener(const SDL_MouseButtonFlags button) {
  guiContext.mouseButtonDownListener(button);
}

void GUIController::addKeyListener(const std::string &id, const SDL_Keycode key,
                                   std::function<void()> listener) {
  guiContext.addKeyListener(id, key, listener);
}

void GUIController::addMouseButtonListener(const std::string &id,
                                           const SDL_MouseButtonFlags button,
                                           std::function<void()> listener) {
  guiContext.addMouseButtonListener(id, button, listener);
}

void GUIController::setMainMenuVisible(const bool visible) {
  guiContext.setComponentVisible("main_menu", visible);
}

void GUIController::setCharacterMenuVisible(const bool visible) {
  guiContext.setComponentVisible("char_menu", visible);
}

int GUIController::run() {
  guiContext.update();

  return 0;
}
