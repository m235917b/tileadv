#include <SDL3/SDL.h>

#include "guicontroller.hpp"
#include "utils/rendercontext.hpp"
#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"

GUIController::GUIController(const RenderContext& renderContext)
    : guiContext(renderContext)
{
    // Initialize main menu elements
    auto mainMenu = std::make_unique<GUIMenu>("main_menu");

    auto mainMenuContainer = std::make_unique<GUIContainer>("main_menu_container", Layout::VERTICAL, 1000, 500, 500, 200);
    mainMenuContainer->setBorder(true);

    auto headline = std::make_unique<GUIElement>("1_headline", ElementType::TEXT, 100, 50, "Main Menu");
    auto line1 = std::make_unique<GUIElement>("2_play", ElementType::TEXT, 100, 50, "Play Game");
    auto line2 = std::make_unique<GUIElement>("3_exit", ElementType::TEXT, 100, 50, "Exit Game");

    mainMenuContainer->addElement(std::move(headline));
    mainMenuContainer->addElement(std::move(line1));
    mainMenuContainer->addElement(std::move(line2));

    mainMenu->addMenuItem(std::move(mainMenuContainer));

    guiContext.addMenu("mainmenu", std::move(mainMenu));
}

bool GUIController::init()
{
    return guiContext.init();
}

void GUIController::initGameMenus()
{
    auto characterMenu = std::make_unique<GUIMenu>("char_menu");

    auto characterMenuContainer = std::make_unique<GUIContainer>("char_menu_container", Layout::VERTICAL, 500, 100, 600, 200);
    characterMenuContainer->setBorder(true);
    characterMenuContainer->setBackground(true);

    auto characterMenuContainer2 = std::make_unique<GUIContainer>("char_menu_container_2", Layout::VERTICAL, 1200, 100, 600, 200);
    characterMenuContainer->setBorder(true);

    auto headline = std::make_unique<GUIElement>("char_headline", ElementType::TEXT, 100, 20, "Character Menu");

    characterMenuContainer->addElement(std::move(headline));

    auto headline2 = std::make_unique<GUIElement>("char_headline_2", ElementType::TEXT, 100, 20, "Character Menu 2");

    characterMenuContainer2->addElement(std::move(headline2));

    characterMenu->addMenuItem(std::move(characterMenuContainer));
    characterMenu->addMenuItem(std::move(characterMenuContainer2));

    guiContext.addMenu("charactermenu", std::move(characterMenu));
}

void GUIController::keyDownListener(const SDL_Keycode key)
{
    guiContext.keyDownListener(key);
}

void GUIController::addKeyListener(const std::string& id, const SDL_Keycode key, std::function<void()> listener)
{
    guiContext.addKeyListener(id, key, listener);
}

void GUIController::setMainMenuVisible(const bool visible)
{
    guiContext.setMenuVisible("mainmenu", visible);
}

void GUIController::setCharacterMenuVisible(const bool visible)
{
    guiContext.setMenuVisible("charactermenu", visible);
}

int GUIController::run()
{
    guiContext.drawGUI();

    return 0;
}
