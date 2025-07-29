#include <SDL3/SDL.h>

#include "graphicsmanager.hpp"

GraphicsManmager::GraphicsManmager()
    : screenWidth(2560), screenHeight(1440), window(nullptr),
      renderer(nullptr) {}

bool GraphicsManmager::init() {
  if (SDL_Init(SDL_INIT_VIDEO) == false) {
    SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
    return false;
  }

  if (SDL_CreateWindowAndRenderer("TileADV", screenWidth, screenHeight, 0,
                                  &window, &renderer) == false) {
    SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
    return false;
  }

  if (SDL_SetWindowFullscreen(window, true) == false) {
    SDL_Log("Could not set fullscreen mode! SDL error: %s\n", SDL_GetError());
    return false;
  }

  /*if (SDL_HideCursor() == false) {
    SDL_Log("Could not hide cursor! SDL error: %s\n", SDL_GetError());
    return false;
  }*/

  /*
  if(SDL_SetRenderVSync(renderer, 1) == false)
  {
      SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
      success = false;
  }
  */

  return true;
}

int GraphicsManmager::destroy() {
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
