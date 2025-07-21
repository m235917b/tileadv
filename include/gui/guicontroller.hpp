#pragma once

#include "gui/guimenu.hpp"
#include "view/view.hpp"
#include "model/chunk.hpp"
#include "model/character.hpp"

enum class GUIState
{
    NONE,
    MAIN_MENU,
    CHARACTER_MENU
};

class GUIController
{
    public:
        GUIController(View* view);

        void initMainMenus();
        void initGameMenus();

        void keyDownListener(const SDL_Keycode& key);

        void showMainMenu();
        void showCharacterMenu(Chunk& chunk, const std::vector<Character*>& characters, const Character* player);

    private:
        GUIState currentState;

        View* view;

        GUIMenu mainMenu;
        GUIMenu characterMenu;
};
