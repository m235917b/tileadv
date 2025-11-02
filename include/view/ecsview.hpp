#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "utils/rendercontext.hpp"

namespace ecsview {

constexpr int screenWidth{2560};
constexpr int screenHeight{1440};
const std::string cursorTexturePath{"guiassets/cursor.png"};

struct RenderContext {
  int screenWidth;
  int screenHeight;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Cursor *cursor;
  SDL_Surface *cursorSurf;
  std::string cursorTexturePath;
};

class GUIRenderContextWrapper : public utils::RenderContext {
public:
  GUIRenderContextWrapper(const ecsview::RenderContext &renderContext);
  ~GUIRenderContextWrapper() = default;

  SDL_Renderer &getRenderer() const;
  int getScreenWidth() const;
  int getScreenHeight() const;

private:
  const ecsview::RenderContext &renderContext;
};

bool init(ecsview::RenderContext &renderContext);

int destroy(ecsview::RenderContext &renderContext);

} // namespace ecsview
