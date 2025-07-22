#include "gui/guicontroller.hpp"
#include "gui/guielement.hpp"

GUIController::GUIController(View* view)
    : currentState(GUIState::MAIN_MENU), view(view), mainMenu(), characterMenu()
{
    // Initialize main menu elements
    auto mainMenuContainer = std::make_unique<GUIContainer>(Layout::VERTICAL, 1000, 500, 100, 200);

    auto headline = std::make_unique<GUIElement>(ElementType::TEXT, 100, 50, "Main Menu");
    auto line1 = std::make_unique<GUIElement>(ElementType::TEXT, 100, 50, "Line 1");
    auto line2 = std::make_unique<GUIElement>(ElementType::TEXT, 100, 50, "Line 2");

    mainMenuContainer->addElement(std::move(headline));
    mainMenuContainer->addElement(std::move(line1));
    mainMenuContainer->addElement(std::move(line2));

    mainMenu.addMenuItem(std::move(mainMenuContainer));
}

void GUIController::initGameMenus()
{
    auto characterMenuContainer = std::make_unique<GUIContainer>(Layout::VERTICAL, 1000, 100, 100, 200);

    auto headline = std::make_unique<GUIElement>(ElementType::TEXT, 100, 20, "Character Menu");

    characterMenuContainer->addElement(std::move(headline));

    characterMenu.addMenuItem(std::move(characterMenuContainer));
}

void GUIController::keyDownListener(const SDL_Keycode& key)
{
    switch (currentState)
    {
        case GUIState::MAIN_MENU:
            mainMenu.keyDownListener(key);
            break;

        case GUIState::CHARACTER_MENU:
            characterMenu.keyDownListener(key);
            break;

        default:
            break;
    }
}

void GUIController::showMainMenu()
{
    currentState = GUIState::MAIN_MENU;
    mainMenu.update();
    view->renderMainMenu(mainMenu);
}

void GUIController::showCharacterMenu(Chunk& chunk, const std::vector<Character*>& characters, const Character* player)
{
    currentState = GUIState::CHARACTER_MENU;
    characterMenu.update();
    view->renderCharacterMenu(characterMenu, chunk, characters, player);
}
