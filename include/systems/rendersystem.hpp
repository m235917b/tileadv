#pragma once

#include <string>

class ECSContext;
class GUIContext;
struct RenderContext;

constexpr float tileSpriteSize{25.f};

std::string registerRenderSystem(const std::string &phase, ECSContext &context);
