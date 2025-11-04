#pragma once

#include <SDL3/SDL.h>

#include "ecs/ecsapi.hpp"
#include "ecs/ecscontext.hpp"
#include "ecsgui/ecsgui.hpp"
#include "gui/guicontext.hpp"
#include "view/view.hpp"

struct SystemRegEntry;

constexpr int framerate{144};

class ASC {
public:
  ASC();
  ~ASC() = default;

  void run();

private:
  ECSContext ecsContext;
  ECSAPI ecsApi;
  RenderContext renderContext;
  GUIRenderContextWrapper renderContextWrapper;
  GUIContext guiContext;

  void initASC();
  void initPhases();
  void initModuleSystems(const std::vector<SystemRegEntry> &systems);
  void initSystems();
  void destroy();
};
