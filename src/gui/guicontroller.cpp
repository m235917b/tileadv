#include "gui/guicontroller.hpp"
#include "gui/listcontainer.hpp"
#include "gui/textlabel.hpp"

GUIController::GUIController(View* view)
    : currentState(GUIState::MAIN_MENU), view(view), mainMenu(), characterMenu()
{
    // Initialize main menu elements
    auto mainMenuContainer = std::make_unique<ListContainer>(10, 10, 100, 200);

    auto headline = std::make_unique<TextLabel>(100, 10, "Main Menu");

    mainMenuContainer->addElement(std::move(headline));

    mainMenu.addMenuItem(std::move(mainMenuContainer));
}

void GUIController::initGameMenus()
{
    auto characterMenuContainer = std::make_unique<ListContainer>(50, 50, 100, 200);

    auto headline = std::make_unique<TextLabel>(100, 20, "Character Menu");

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
    view->renderMainMenu();
}

void GUIController::showCharacterMenu(Chunk& chunk, const std::vector<Character*>& characters, const Character* player)
{
    currentState = GUIState::CHARACTER_MENU;
    characterMenu.update();
    view->renderCharacterMenu(chunk, characters, player);
}
