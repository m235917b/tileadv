#pragma once

enum class ApplicationState { RUNNING, MAIN_MENU, QUIT };

struct ApplicationStateResource {
  ApplicationState state;
};
