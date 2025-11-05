#include "view/resources.hpp"

#include "ecs/ecs.hpp"
#include "gui/guicontext.hpp"
#include "view/resources.hpp"
#include "view/view.hpp"

void initRenderContextResource(ECSContext &ecsContext,
                               const RenderContext &renderContext) {
  ecsContext.getCommandBuffer().upsertResource(
      RenderContextResource{&renderContext});
}

void initCameraResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(CameraResource{
      0, 0, leftMargin, topMargin, tileSize, cameraMarginX, cameraMarginY});
}

void initViewResources(ECSContext &ecsContext, RenderContext &renderContext) {
  initRenderContextResource(ecsContext, renderContext);
  initCameraResource(ecsContext);
}
