#pragma once

#include <string>

#include <SDL3/SDL.h>

#include "utils/rendercontext.hpp"

struct SDL_Window;
struct SDL_Renderer;

class GraphicsManmager : public RenderContext {
public:
  GraphicsManmager();
  GraphicsManmager(const GraphicsManmager &) = delete;
  GraphicsManmager &operator=(const GraphicsManmager &) = delete;
  GraphicsManmager &operator=(GraphicsManmager &&) = delete;
  ~GraphicsManmager() = default;

  bool init();
  int destroy();

  void beginFrame();
  void endFrame();

  int getScreenWidth() const override;
  int getScreenHeight() const override;
  SDL_Renderer &getRenderer() const override;

private:
  int screenWidth;
  int screenHeight;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Cursor *cursor;
  SDL_Surface *cursorSurf;
  std::string cursorTexturePath;
};
