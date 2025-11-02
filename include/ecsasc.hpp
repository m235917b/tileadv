#pragma once

#include <SDL3/SDL.h>

#include "ecs/ecsapi.hpp"
#include "ecs/ecscontext.hpp"
#include "gui/guicontext.hpp"
#include "view/ecsview.hpp"

constexpr int framerate{144};

class ECSASC {
public:
  ECSASC();
  ~ECSASC() = default;

  void init();
  void run();
  void destroy();

private:
  ECSContext ecsContext;
  ECSAPI ecsApi;
  ecsview::RenderContext renderContext;
  ecsview::GUIRenderContextWrapper renderContextWrapper;
  GUIContext guiContext;
};
