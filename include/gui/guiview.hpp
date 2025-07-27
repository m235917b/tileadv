#pragma once

#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

#include "utils/ltexture.hpp"
#include "utils/rendercontext.hpp"

struct SDL_Renderer;

class GUIComponent;

class GUIView {
public:
  GUIView(const RenderContext &renderContext);

  template <std::ranges::input_range Range>
    requires std::same_as<std::ranges::range_value_t<Range>, std::string>
  bool loadTextures(const Range &texturePaths) {
    SDL_Renderer &renderer{renderContext.getRenderer()};

    SDL_SetRenderDrawBlendMode(&renderer, SDL_BLENDMODE_BLEND);

    if (!asciiGrey.loadFromFile("guiassets/ascii_grey.png", renderer)) {
      SDL_Log("Unable to load png image!\n");

      return false;
    }

    for (const auto &path : texturePaths) {
      if (images.try_emplace(path).second) {
        if (!images[path].loadFromFile(path, renderer)) {
          return false;
        }
      }
    }

    return true;
  }

  void drawGUIComponent(GUIComponent &component, const std::string &selected);
  void drawText(const float posX, const float posY, const float width,
                const float height, const std::string &text);
  void drawImage(const float posX, const float posY, const float width,
                 const float height, const std::string &path);

private:
  const RenderContext &renderContext;

  LTexture asciiGrey;

  std::unordered_map<std::string, LTexture> images;

  float borderMargin;
};
