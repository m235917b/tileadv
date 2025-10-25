#include "utils/timer.hpp"

Timer::Timer(Uint64 intervalMs)
    : intervalMs(intervalMs), startTick(0), listener() {}

void Timer::run() {
  Uint64 currentTick = SDL_GetTicks();
  if (currentTick - startTick >= intervalMs) {
    startTick = currentTick;
    if (listener) {
      listener();
    }
  }
}

void Timer::setListener(std::function<void()> listener) {
  this->listener = listener;
}

DeltaTimer::DeltaTimer(Uint64 intervalMs)
    : intervalMs(intervalMs), lastCall(0) {}

bool DeltaTimer::intervalElapsed() {
  Uint64 currentTick = SDL_GetTicks();

  if (currentTick - lastCall >= intervalMs) {
    lastCall = currentTick;
    return true;
  }

  return false;
}
