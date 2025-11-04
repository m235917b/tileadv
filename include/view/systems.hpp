#pragma once

#include <vector>

#include "asc/systemregistry.hpp"

class ECSContext;
class GUIContext;
struct RenderContext;

constexpr float tileSpriteSize{25.f};

std::vector<SystemRegEntry> getViewSystems();
