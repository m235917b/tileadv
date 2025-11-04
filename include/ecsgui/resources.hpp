class GUIContext;
class ECSContext;

struct GUIContextResource {
  const GUIContext *context;
};

void initECSGUIResources(ECSContext &ecsContext, GUIContext &guiContext);
