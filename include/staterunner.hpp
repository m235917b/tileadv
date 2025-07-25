#pragma once

class StateRunner {
public:
  StateRunner() = default;

  virtual int run() = 0;
};