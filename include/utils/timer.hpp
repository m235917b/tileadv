#pragma once

#include <functional>

#include <SDL3/SDL.h>

class Timer {
public:
  Timer(Uint64 intervalMs);

  void run();
  void setListener(std::function<void()> listener);

private:
  Uint64 intervalMs;
  Uint64 startTick;
  std::function<void()> listener;
};

class DeltaTimer {
public:
  DeltaTimer(Uint64 intervalMs);

  bool intervalElapsed();

private:
  Uint64 intervalMs;
  Uint64 lastCall;
};
