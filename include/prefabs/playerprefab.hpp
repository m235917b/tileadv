#pragma once

#include "components/actor.hpp"
#include "components/position.hpp"

class ECSAPI;

constexpr Actor playerActorPrefab = Actor{ActorType::PLAYER};
constexpr Position playerPositionPrefab = Position{60, 30};

void registerPlayerPrefab(ECSAPI &ecsApi);
