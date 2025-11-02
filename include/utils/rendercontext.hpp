#pragma once

struct SDL_Renderer;

namespace utils {
class RenderContext {
public:
  virtual SDL_Renderer &getRenderer() const = 0;
  virtual int getScreenWidth() const = 0;
  virtual int getScreenHeight() const = 0;
};
} // namespace utils
