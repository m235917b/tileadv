#pragma once

class ECSContext;

enum class ApplicationState { RUNNING, MAIN_MENU, QUIT };

struct ApplicationStateResource {
  ApplicationState state;
};

void initApplicationStateResource(ECSContext &ecsContext);
