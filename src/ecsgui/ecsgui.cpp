#include "ecsgui/ecsgui.hpp"

#include "ecsgui/resources.hpp"

GUIRenderContextWrapper::GUIRenderContextWrapper(
    const RenderContext &renderContext)
    : renderContext(renderContext) {}

SDL_Renderer &GUIRenderContextWrapper::getRenderer() const {
  return *renderContext.renderer;
}

int GUIRenderContextWrapper::getScreenWidth() const {
  return renderContext.screenWidth;
}

int GUIRenderContextWrapper::getScreenHeight() const {
  return renderContext.screenHeight;
}

void initECSGUI(ECSContext &ecsContext, GUIContext &guiContext) {
  initECSGUIResources(ecsContext, guiContext);
}
