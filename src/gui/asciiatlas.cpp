#include <SDL3/SDL.h>

#include "gui/asciiatlas.hpp"

SDL_FRect AsciiAtlas::getSpriteCoords(const char asciiChar) {
  // Calculate the sprite coordinates based on the ASCII character
  return {(asciiChar - 32) * asciiWidth, 0, asciiWidth, asciiHeight};
}
