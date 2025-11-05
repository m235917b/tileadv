#pragma once

class ECSContext;
class ECSAPI;

enum class ApplicationState { RUNNING, MAIN_MENU, QUIT };

struct ApplicationStateResource {
  ApplicationState state;
};

struct ECSAPIResource {
  ECSAPI *ecsApi;
};

struct KeyMapResource {
  bool w;
  bool a;
  bool s;
  bool d;
};

struct MousePosResource {
  float x;
  float y;
  int tileX;
  int tileY;
};

void initASCResources(ECSContext &ecsContext, ECSAPI &ecsApi);
