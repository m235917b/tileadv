#pragma once

#include <SDL3/SDL.h>

#include "gui/gui.hpp"
#include "view/view.hpp"

class GUIRenderContextWrapper : public GUIRenderContext {
public:
  GUIRenderContextWrapper(const RenderContext &renderContext);
  ~GUIRenderContextWrapper() = default;

  SDL_Renderer &getRenderer() const;
  int getScreenWidth() const;
  int getScreenHeight() const;

private:
  const RenderContext &renderContext;
};

void initECSGUI(ECSContext &ecsContext, GUIContext &guiContext);
