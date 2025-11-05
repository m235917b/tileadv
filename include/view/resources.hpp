#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

class ECSContext;
struct RenderContext;

struct TextureResource {
  SDL_Texture *tiles;
  SDL_Texture *actors;
};

struct RenderContextResource {
  const RenderContext *context;
};

struct CameraResource {
  int posX;
  int posY;
  int leftMargin;
  int topMargin;
  int tileSize;
  int cameraMarginX;
  int cameraMarginY;
};

void initViewResources(ECSContext &ecsContext, RenderContext &renderContext);
