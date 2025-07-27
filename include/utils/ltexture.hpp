#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_FRect;

class LTexture {
public:
  static constexpr float kOriginalSize = -1.f;

  LTexture();
  LTexture(const LTexture &) = delete;
  LTexture &operator=(const LTexture &) = delete;
  LTexture(LTexture &&) noexcept;
  LTexture &operator=(LTexture &&) noexcept;
  ~LTexture();

  bool loadFromFile(const std::string &path, SDL_Renderer &renderer);

  void destroy();

  void render(const float x, const float y, const SDL_FRect *clip,
              const float width, const float height, SDL_Renderer &renderer);

  int getWidth() const;
  int getHeight() const;
  bool isLoaded() const;

private:
  SDL_Texture *texture;

  int width;
  int height;
};