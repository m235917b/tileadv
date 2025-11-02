#pragma once

#include <SDL3/SDL.h>

#include "ecs/ecsapi.hpp"
#include "ecs/ecscontext.hpp"
#include "gui/guicontext.hpp"
#include "view/view.hpp"

constexpr int framerate{144};

class ECSASC {
public:
  ECSASC();
  ~ECSASC() = default;

  void run();

private:
  ECSContext ecsContext;
  ECSAPI ecsApi;
  RenderContext renderContext;
  GUIRenderContextWrapper renderContextWrapper;
  GUIContext guiContext;

  void initSDL();
  void initResources();
  void initPhases();
  void initSystems();
  void initEventListeners();
  void initCommandHandlers();
  void destroy();
};
