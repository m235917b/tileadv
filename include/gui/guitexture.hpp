#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_FRect;

class GUITexture {
public:
  static constexpr float kOriginalSize = -1.f;

  GUITexture();
  GUITexture(const GUITexture &) = delete;
  GUITexture &operator=(const GUITexture &) = delete;
  GUITexture(GUITexture &&) noexcept;
  GUITexture &operator=(GUITexture &&) noexcept;
  ~GUITexture();

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