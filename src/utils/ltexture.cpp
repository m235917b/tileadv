#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "utils/ltexture.hpp"

LTexture::LTexture() : texture{nullptr}, width{0}, height{0} {}

LTexture::LTexture(LTexture &&other) noexcept
    : texture(other.texture), width(other.width), height(other.height) {
  other.texture = nullptr;
  other.width = 0;
  other.height = 0;
}

LTexture &LTexture::operator=(LTexture &&other) noexcept {
  if (this != &other) {
    destroy();
    texture = other.texture;
    width = other.width;
    height = other.height;
    other.texture = nullptr;
    other.width = 0;
    other.height = 0;
  }
  return *this;
}

LTexture::~LTexture() { destroy(); }

bool LTexture::loadFromFile(const std::string &path, SDL_Renderer &renderer) {
  destroy();

  if (SDL_Surface *loadedSurface{IMG_Load(path.c_str())};
      loadedSurface == nullptr) {
    SDL_Log("Unable to load image %s! SDL_image error: %s\n", path.c_str(),
            SDL_GetError());
  } else {
    if (SDL_SetSurfaceColorKey(
            loadedSurface, true,
            SDL_MapSurfaceRGB(loadedSurface, 0xFF, 0x00, 0xFF)) == false) {
      SDL_Log("Unable to color key! SDL error: %s", SDL_GetError());
    } else {
      if (texture = SDL_CreateTextureFromSurface(&renderer, loadedSurface);
          texture == nullptr) {
        SDL_Log("Unable to create texture from loaded pixels! SDL error: %s\n",
                SDL_GetError());
      } else {
        width = loadedSurface->w;
        height = loadedSurface->h;
      }
    }

    SDL_DestroySurface(loadedSurface);
  }

  return texture != nullptr;
}

void LTexture::destroy() {
  SDL_DestroyTexture(texture);
  texture = nullptr;
  width = 0;
  height = 0;
}

void LTexture::render(const float x, const float y, const SDL_FRect *clip,
                      const float width, const float height,
                      SDL_Renderer &renderer) {
  const SDL_FRect dstRect{x, y, width > 0 ? width : clip->w,
                          height > 0 ? height : clip->h};

  SDL_RenderTexture(&renderer, texture, clip, &dstRect);
}

int LTexture::getWidth() const { return width; }

int LTexture::getHeight() const { return height; }

bool LTexture::isLoaded() const { return texture != nullptr; }
