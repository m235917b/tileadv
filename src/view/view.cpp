#include "view/view.hpp"

#include "ecs/ecs.hpp"
#include "view/resources.hpp"

SDL_Texture *loadTextureFromFile(const std::string &path,
                                 SDL_Renderer &renderer) {
  SDL_Texture *texture;

  SDL_Surface *loadedSurface{IMG_Load(path.c_str())};

  if (!loadedSurface) {
    return nullptr;
  }

  if (SDL_SetSurfaceColorKey(
          loadedSurface, true,
          SDL_MapSurfaceRGB(loadedSurface, 0xFF, 0x00, 0xFF)) == false) {
    return nullptr;
  }

  texture = SDL_CreateTextureFromSurface(&renderer, loadedSurface);

  SDL_DestroySurface(loadedSurface);

  return texture;
}

bool loadTextures(ECSContext &ecsContext, RenderContext &renderContext) {
  auto *tileTexture{
      loadTextureFromFile("assets/tiles_world.png", *renderContext.renderer)};

  if (!tileTexture) {
    return false;
  }

  auto *actorTexture{
      loadTextureFromFile("assets/tiles_actor.png", *renderContext.renderer)};

  if (!actorTexture) {
    return false;
  }

  ecsContext.getCommandBuffer().upsertResource(
      TextureResource{tileTexture, actorTexture});

  return true;
}

bool initView(ECSContext &ecsContext, RenderContext &renderContext) {
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

  initViewResources(ecsContext, renderContext);
  loadTextures(ecsContext, renderContext);

  return true;
}

void destroyTextures(ECSContext &ecsContext) {
  // TODO: Destroy via a mutating command!
  const auto &textures{
      ecsContext.getResourceManager().getResource<TextureResource>()};
  SDL_DestroyTexture(textures->tiles);
  /*SDL_DestroyTexture(texture);
  texture = nullptr;*/
}

int destroyView(ECSContext &ecsContext, RenderContext &renderContext) {
  destroyTextures(ecsContext);

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
