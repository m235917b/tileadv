#pragma once

#include <SDL3/SDL_keycode.h>

class ECSContext;

struct KeyDownEvent {
  SDL_Keycode keycode;
};

struct KeyUpEvent {
  SDL_Keycode keycode;
};

void initASCEvents(ECSContext &ecsContext);
