#pragma once

#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

#include "gui/guirendercontext.hpp"
#include "utils/ltexture.hpp"


struct SDL_Renderer;

class GUIComponent;

enum class GUIFittingMode;

class GUIView {
public:
  GUIView(const GUIRenderContext &renderContext);

  bool loadTextures(const std::vector<std::string> &texturePaths);

  GUIComponent *drawAndHitTest(GUIComponent &component,
                               const std::string &selected, const float mouseX,
                               const float mouseY);
  void drawText(const SDL_FRect rect, const std::string &text, const float size,
                GUIFittingMode fittingMode, const bool centerLeft,
                const bool centerTop, const uint32_t color);
  void drawImage(const SDL_FRect rect, const std::string &path,
                 const float size, GUIFittingMode fittingMode,
                 const bool centerLeft, const bool centerTop);

private:
  const GUIRenderContext &renderContext;
  LTexture asciiGrey;
  std::unordered_map<std::string, LTexture> images;
  const uint32_t selectedColor;
};
