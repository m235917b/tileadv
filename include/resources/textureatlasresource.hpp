#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

struct TileType;

struct TextureAtlasResource {
  std::unordered_map<std::string, SDL_Texture *> atlas;
};

struct TileAtlasResource {
  std::unordered_map<TileType, SDL_Texture *> atlas;
};
