#include "view/view.hpp"

GUIRenderContextWrapper::GUIRenderContextWrapper(
    const RenderContext &renderContext)
    : renderContext(renderContext) {}

SDL_Renderer &GUIRenderContextWrapper::getRenderer() const {
  return *renderContext.renderer;
}

int GUIRenderContextWrapper::getScreenWidth() const {
  return renderContext.screenWidth;
}

int GUIRenderContextWrapper::getScreenHeight() const {
  return renderContext.screenHeight;
}

bool initView(RenderContext &renderContext) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Cursor *cursor;
  SDL_Surface *cursorSurf;

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

  renderContext.screenWidth = screenWidth;
  renderContext.screenHeight = screenHeight;
  renderContext.window = window;
  renderContext.renderer = renderer;
  renderContext.cursor = cursor;
  renderContext.cursorSurf = cursorSurf;
  renderContext.cursorTexturePath = cursorTexturePath;

  return true;
}

int destroyView(RenderContext &renderContext) {
  SDL_DestroyCursor(renderContext.cursor);
  renderContext.cursor = nullptr;
  SDL_DestroySurface(renderContext.cursorSurf);
  renderContext.cursorSurf = nullptr;
  SDL_DestroyRenderer(renderContext.renderer);
  renderContext.renderer = nullptr;
  SDL_DestroyWindow(renderContext.window);
  renderContext.window = nullptr;

  SDL_Quit();

  return 0;
}
