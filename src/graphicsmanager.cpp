#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "graphicsmanager.hpp"

GraphicsManmager::GraphicsManmager()
    : screenWidth(2560), screenHeight(1440), window(nullptr), renderer(nullptr),
      cursor(nullptr), cursorSurf(nullptr),
      cursorTexturePath("guiassets/cursor.png") {}

bool GraphicsManmager::init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_CreateWindowAndRenderer("TileADV", screenWidth, screenHeight, 0,
                                   &window, &renderer)) {
    SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_SetWindowFullscreen(window, true)) {
    SDL_Log("Could not set fullscreen mode! SDL error: %s\n", SDL_GetError());
    return false;
  }

  /*if (!SDL_HideCursor()) {
    SDL_Log("Could not hide cursor! SDL error: %s\n", SDL_GetError());
    return false;
  }*/

  cursorSurf = IMG_Load(cursorTexturePath.c_str());

  if (cursorSurf == nullptr) {
    SDL_Log("Could not load cursor image! SDL error: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_SetSurfaceColorKey(
          cursorSurf, true, SDL_MapSurfaceRGB(cursorSurf, 0xFF, 0x00, 0xFF))) {
    SDL_Log("Unable to color key! SDL error: %s", SDL_GetError());
    return false;
  }

  cursor = SDL_CreateColorCursor(cursorSurf, 0, 0);

  if (cursor == nullptr) {
    SDL_Log("Could not create cursor! SDL error: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_SetCursor(cursor)) {
    SDL_Log("Could not set cursor! SDL error: %s\n", SDL_GetError());
    return false;
  }

  /*
  if(!SDL_SetRenderVSync(renderer, 1))
  {
      SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
      success = false;
  }
  */

  return true;
}

int GraphicsManmager::destroy() {
  SDL_DestroyCursor(cursor);
  cursor = nullptr;
  SDL_DestroySurface(cursorSurf);
  cursorSurf = nullptr;
  SDL_DestroyRenderer(renderer);
  renderer = nullptr;
  SDL_DestroyWindow(window);
  window = nullptr;

  SDL_Quit();

  return 0;
}

void GraphicsManmager::beginFrame() {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer);
}

void GraphicsManmager::endFrame() { SDL_RenderPresent(renderer); }

int GraphicsManmager::getScreenWidth() const { return screenWidth; }

int GraphicsManmager::getScreenHeight() const { return screenHeight; }

SDL_Renderer &GraphicsManmager::getRenderer() const { return *renderer; }
