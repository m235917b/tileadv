#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "gui/gui.hpp"

constexpr int screenWidth{2560};
constexpr int screenHeight{1440};
const std::string cursorTexturePath{"guiassets/cursor.png"};

class ECSContext;

struct RenderContext {
  int screenWidth;
  int screenHeight;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Cursor *cursor;
  SDL_Surface *cursorSurf;
  std::string cursorTexturePath;
};

class GUIRenderContextWrapper : public GUIRenderContext {
public:
  GUIRenderContextWrapper(const RenderContext &renderContext);
  ~GUIRenderContextWrapper() = default;

  SDL_Renderer &getRenderer() const;
  int getScreenWidth() const;
  int getScreenHeight() const;

private:
  const RenderContext &renderContext;
};

bool initView(RenderContext &renderContext);

bool loadTextures(RenderContext &renderContext, ECSContext &ecsContext);

void destroyTextures(ECSContext &ecsContext);

int destroyView(RenderContext &renderContext);
