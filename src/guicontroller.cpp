#include <SDL3/SDL.h>

#include "gui/guicomponent.hpp"
#include "guicontroller.hpp"
#include "utils/rendercontext.hpp"

GUIController::GUIController(const RenderContext &renderContext)
    : guiContext(renderContext) {
  // Initialize main menu elements
  auto mainMenu{
      std::make_unique<GUIComponent>("main_menu", 1000, 500, 500, 200)};
  mainMenu->setVisible(false);
  mainMenu->setLayout(GUILayout::VERTICAL);
  mainMenu->setBorder(true);

  auto headline{std::make_unique<GUIComponent>("1_headline", 0, 0, 100, 50)};
  headline->setType(GUIElementType::ELEMENT);
  headline->setText("Main Menu");
  auto line1{std::make_unique<GUIComponent>("2_play", 0, 0, 100, 50)};
  line1->setType(GUIElementType::ELEMENT);
  line1->setText("Play Game");
  auto line2{std::make_unique<GUIComponent>("3_exit", 0, 0, 100, 50)};
  line2->setType(GUIElementType::ELEMENT);
  line2->setText("Exit Game");

  mainMenu->addChild(std::move(headline));
  mainMenu->addChild(std::move(line1));
  mainMenu->addChild(std::move(line2));

  guiContext.addComponent(std::move(mainMenu));
}

bool GUIController::init() { return guiContext.init(); }

void GUIController::initGameMenus() {
  auto characterMenu{std::make_unique<GUIComponent>("char_menu")};
  characterMenu->setVisible(false);

  auto characterMenuContainer{std::make_unique<GUIComponent>(
      "char_menu_container", 500, 100, 600, 200)};
  characterMenuContainer->setLayout(GUILayout::VERTICAL);
  characterMenuContainer->setBorder(true);
  characterMenuContainer->setBackground(true);

  auto characterMenuContainer2{std::make_unique<GUIComponent>(
      "char_menu_container_2", 1200, 100, 600, 200)};
  characterMenuContainer2->setLayout(GUILayout::VERTICAL);
  characterMenuContainer2->setImage("guiassets/border_brown.png");

  auto headline{
      std::make_unique<GUIComponent>("char_headline", 0, 0, 100, 200)};
  headline->setType(GUIElementType::ELEMENT);
  headline->setText("Character Menu");

  characterMenuContainer->addChild(std::move(headline));

  auto headline2{
      std::make_unique<GUIComponent>("char_headline_2", 10, 10, 300, 100)};
  headline2->setType(GUIElementType::ELEMENT);
  headline2->setImage("guiassets/test_button.png");

  characterMenuContainer2->addChild(std::move(headline2));

  auto characterMenuContainer3{std::make_unique<GUIComponent>(
      "char_menu_container_3", 900, 500, 600, 800)};
  characterMenuContainer3->setLayout(GUILayout::VERTICAL);
  characterMenuContainer3->setImage("guiassets/border_brown.png");

  auto subcontainer1{std::make_unique<GUIComponent>(
      "sub_container_1", 10, 10, 580, 300)};
  subcontainer1->setLayout(GUILayout::VERTICAL);
  subcontainer1->setImage("guiassets/border_brown.png");

  auto line1{
      std::make_unique<GUIComponent>("line_1", 0, 0, 100, 30)};
  line1->setType(GUIElementType::ELEMENT);
  line1->setText("Line 1");

  auto line2{
      std::make_unique<GUIComponent>("line_2", 0, 0, 100, 30)};
  line2->setType(GUIElementType::ELEMENT);
  line2->setText("Line 2");

  subcontainer1->addChild(std::move(line1));
  subcontainer1->addChild(std::move(line2));

  auto subcontainer2{std::make_unique<GUIComponent>(
      "sub_container_2", 10, 10, 500, 200)};
  subcontainer2->setLayout(GUILayout::VERTICAL);
  subcontainer2->setImage("guiassets/border_brown.png");

  auto line3{
      std::make_unique<GUIComponent>("line_3", 0, 0, 100, 30)};
  line3->setType(GUIElementType::ELEMENT);
  line3->setText("Line 3");

  auto line4{
      std::make_unique<GUIComponent>("line_4", 0, 0, 100, 30)};
  line4->setType(GUIElementType::ELEMENT);
  line4->setText("Line 4");

  subcontainer2->addChild(std::move(line3));
  subcontainer2->addChild(std::move(line4));

  auto subcontainer3{std::make_unique<GUIComponent>(
      "sub_container_3", 10, 10, 580, 300)};
  subcontainer3->setLayout(GUILayout::VERTICAL);
  subcontainer3->setImage("guiassets/border_brown.png");

  subcontainer3->addChild(std::move(subcontainer2));

  characterMenuContainer3->addChild(std::move(subcontainer1));
  characterMenuContainer3->addChild(std::move(subcontainer3));

  characterMenu->addChild(std::move(characterMenuContainer));
  characterMenu->addChild(std::move(characterMenuContainer2));
  characterMenu->addChild(std::move(characterMenuContainer3));

  guiContext.addComponent(std::move(characterMenu));
}

void GUIController::keyDownListener(const SDL_Keycode key) {
  guiContext.keyDownListener(key);
}

void GUIController::addKeyListener(const std::string &id, const SDL_Keycode key,
                                   std::function<void()> listener) {
  guiContext.addKeyListener(id, key, listener);
}

void GUIController::setMainMenuVisible(const bool visible) {
  guiContext.setComponentVisible("main_menu", visible);
}

void GUIController::setCharacterMenuVisible(const bool visible) {
  guiContext.setComponentVisible("char_menu", visible);
}

int GUIController::run() {
  guiContext.update();
  guiContext.updateLayout();
  guiContext.drawGUI();

  return 0;
}
