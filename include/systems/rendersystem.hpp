#pragma once

#include <string>

#include "view/ecsview.hpp"

class ECSContext;
class RenderContext;
class GUIContext;

std::string registerRenderSystem(const std::string &phase, ECSContext &context,
                                 const ecsview::RenderContext &renderContext,
                                 const GUIContext &guiContext);
