#pragma once

#include <SDL3/SDL_keycode.h>

class ECSContext;

struct GameKeyDownEvent {
  SDL_Keycode keycode;
};

struct KeyDownEvent {
  SDL_Keycode keycode;
};

struct GameKeyUpEvent {
  SDL_Keycode keycode;
};

struct GameMouseDownEvent {
  Uint8 button;
};

struct MouseDownEvent {
  Uint8 button;
};

struct MouseMotionEvent {
  float posX;
  float posY;
  int tileX;
  int tileY;
};

void initASCEvents(ECSContext &ecsContext);
