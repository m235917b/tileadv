#pragma once

#include "gamecontroller.hpp"
#include "graphicsmanager.hpp"
#include "guicontroller.hpp"
#include "view/view.hpp"


constexpr int framerate{60};

enum class ApplicationState { MAIN_MENU, GAMEPLAY };

class ApplicationStateController : public StateRunner {
public:
  ApplicationStateController();
  ApplicationStateController(const ApplicationStateController &) = delete;
  ApplicationStateController &
  operator=(const ApplicationStateController &) = delete;

  int run() override;

private:
  ApplicationState currentState;

  GraphicsManmager graphicsManager;
  View view;
  GUIController guiController;
  GameController gameController;
};