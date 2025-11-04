#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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

bool initView(ECSContext &ecsContext, RenderContext &renderContext);

int destroyView(ECSContext &ecsContext, RenderContext &renderContext);
