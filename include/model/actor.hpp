#pragma once

class Actor {
public:
  Actor();
  virtual ~Actor() = default;

  virtual void update();
};
