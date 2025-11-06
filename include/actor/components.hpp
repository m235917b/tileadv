#pragma once

enum class ActorType { PLAYER, DUMMY, FIREBALL };

struct Actor {
  ActorType type;
};

struct Position {
  int x;
  int y;
};

struct SpeedControl {
  int speed;
  int timeLeft;
};

struct Garbage {
  bool isGarbage;
};

struct Physics {
  bool solid;
};
