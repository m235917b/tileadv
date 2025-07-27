#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "utils/ltexture.hpp"
#include "utils/rendercontext.hpp"

struct SDL_Renderer;

class GUIComponent;

class GUIView {
public:
  GUIView(const RenderContext &renderContext);

  bool init(const std::vector<std::string> &texturePaths);

  void drawGUIComponent(const GUIComponent &component, const bool selected);
  void drawText(const int posX, const int posY, const float size,
                const std::string &text);
  void drawImage(const int posX, const int posY, const int width,
                 const int height, const std::string &path);

private:
  const RenderContext &renderContext;

  LTexture asciiGrey;

  std::unordered_map<std::string, LTexture> images;
};
