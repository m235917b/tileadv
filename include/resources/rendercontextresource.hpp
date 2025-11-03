#pragma once

class ECSContext;
struct RenderContext;

struct RenderContextResource {
  const RenderContext *context;
};

void initRenderContextResource(ECSContext &ecsContext,
                               const RenderContext &renderContext);
