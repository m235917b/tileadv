#include <SDL3/SDL.h>

#include "guicontroller.hpp"
#include "utils/rendercontext.hpp"
#include "gui/guimenu.hpp"
#include "gui/guicontainer.hpp"
#include "gui/guielement.hpp"

GUIController::GUIController(RenderContext* renderContext)
    : guiContext(renderContext)
{
    // Initialize main menu elements
    auto mainMenu = std::make_unique<GUIMenu>();

    auto mainMenuContainer = std::make_unique<GUIContainer>(Layout::VERTICAL, 1000, 500, 100, 200);

    auto headline = std::make_unique<GUIElement>(ElementType::TEXT, 100, 50, "Main Menu");
    auto line1 = std::make_unique<GUIElement>(ElementType::TEXT, 100, 50, "Line 1");
    auto line2 = std::make_unique<GUIElement>(ElementType::TEXT, 100, 50, "Line 2");

    mainMenuContainer->addElement(std::move(headline));
    mainMenuContainer->addElement(std::move(line1));
    mainMenuContainer->addElement(std::move(line2));

    mainMenu->addMenuItem(std::move(mainMenuContainer));

    this->guiContext.addMenu("mainmenu", std::move(mainMenu));
}

bool GUIController::init()
{
    return this->guiContext.init();
}

void GUIController::destroy()
{
    this->guiContext.destroy();
}

void GUIController::initGameMenus()
{
    auto characterMenu = std::make_unique<GUIMenu>();

    auto characterMenuContainer = std::make_unique<GUIContainer>(Layout::VERTICAL, 1000, 100, 100, 200);

    auto headline = std::make_unique<GUIElement>(ElementType::TEXT, 100, 20, "Character Menu");

    characterMenuContainer->addElement(std::move(headline));

    characterMenu->addMenuItem(std::move(characterMenuContainer));

    this->guiContext.addMenu("charactermenu", std::move(characterMenu));
}

void GUIController::keyDownListener(const SDL_Keycode& key)
{
    this->guiContext.keyDownListener(key);
}

void GUIController::setMainMenuVisible(const bool visible)
{
    this->guiContext.setMenuVisible("mainmenu", visible);
}

void GUIController::setCharacterMenuVisible(const bool visible)
{
    this->guiContext.setMenuVisible("charactermenu", visible);
}

void GUIController::drawMenus()
{
    this->guiContext.drawGUI();
}
