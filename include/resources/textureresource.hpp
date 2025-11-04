#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

struct TextureResource {
  SDL_Texture *tiles;
  SDL_Texture *actors;
};
