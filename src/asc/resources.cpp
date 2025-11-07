#include "asc/resources.hpp"

#include <any>

#include "ecs/ecs.hpp"
#include "gui/guicontext.hpp"

void initECSAPIResource(ECSContext &ecsContext, ECSAPI &ecsApi) {
  ecsContext.getCommandBuffer().upsertResource(ECSAPIResource{&ecsApi});
}

void initApplicationStateResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(
      ApplicationStateResource{ApplicationState::RUNNING});
}

void initKeyMapResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(
      KeyMapResource{false, false, false, false});
}

void initMousePosResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(MousePosResource{});
}

void initGUIContextResource(ECSContext &ecsContext, GUIContext &guiContext) {
  ecsContext.getCommandBuffer().upsertResource(
      std::make_any<GUIContextResource>(GUIContextResource{&guiContext}));
}

void initASCResources(ECSContext &ecsContext, ECSAPI &ecsApi,
                      GUIContext &guiContext) {
  initECSAPIResource(ecsContext, ecsApi);
  initGUIContextResource(ecsContext, guiContext);
  initApplicationStateResource(ecsContext);
  initKeyMapResource(ecsContext);
  initMousePosResource(ecsContext);
}
