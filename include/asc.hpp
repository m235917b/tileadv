#pragma once

#include "gamecontroller.hpp"
#include "view/view.hpp"
#include "gui/guicontroller.hpp"

enum class ApplicationState
{
    MAIN_MENU,
    GAMEPLAY
};

class ApplicationStateController : public StateRunner
{
    public:
        ApplicationStateController();
        ApplicationStateController(const ApplicationStateController&) = delete;
        ApplicationStateController& operator=(const ApplicationStateController&) = delete;

        int run() override;

    private:
        ApplicationState currentState;

        GameController gameController;
        View view;
        GUIController guiController;
};