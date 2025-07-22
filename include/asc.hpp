#pragma once

#include "graphicsmanager.hpp"
#include "gamecontroller.hpp"
#include "view/view.hpp"
#include "guicontroller.hpp"

constexpr int framerate{ 60 };

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

        GraphicsManmager graphicsManager;
        View view;
        GUIController guiController;
        GameController gameController;
};