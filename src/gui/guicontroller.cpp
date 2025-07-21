#include "gui/guicontroller.hpp"

GUIController::GUIController(View* view)
    : currentState(GUIState::MAIN_MENU), view(view), mainMenu(), characterMenu()
{
    
}

void GUIController::initMainMenus()
{

}

void GUIController::initGameMenus()
{
    
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
