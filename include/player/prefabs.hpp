#pragma once

#include "actor/components.hpp"

class ECSAPI;

constexpr Actor playerActorPrefab = Actor{ActorType::PLAYER};
constexpr Position playerPositionPrefab = Position{60, 30};
constexpr SpeedControl playerSpeedControl = SpeedControl{100, 0};

void initPlayerPrefabs(ECSAPI &ecsApi);
