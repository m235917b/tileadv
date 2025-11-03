#pragma once

class ECSContext;
class GUIContext;

struct GUIContextResource {
  const GUIContext *context;
};

void initGUIContextResource(ECSContext &ecsContext,
                            const GUIContext &guiContext);
