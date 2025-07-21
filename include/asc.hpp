#pragma once

#include "gamecontroller.hpp"
#include "view/view.hpp"

enum class ApplicationState
{
    MAIN_MENU,
    CHARACTER_MENU,
    GAMEPLAY
};

class ApplicationStateController
{
    public:
        ApplicationStateController();

        int run();

    private:
        ApplicationState currentState;

        GameController gameController;
        View view;
};