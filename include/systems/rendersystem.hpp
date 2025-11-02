#pragma once

#include <string>

class ECSContext;
class GUIContext;
struct RenderContext;

std::string registerRenderSystem(const std::string &phase, ECSContext &context,
                                 const RenderContext &renderContext,
                                 const GUIContext &guiContext);
