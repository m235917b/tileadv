#pragma once

#include "components/actor.hpp"
#include "components/characterattributes.hpp"
#include "components/movecooldown.hpp"
#include "components/position.hpp"

class ECSAPI;

constexpr Actor playerActorPrefab = Actor{ActorType::PLAYER};
constexpr Position playerPositionPrefab = Position{60, 30};
constexpr CharacterAttributes playerCharAttributes = CharacterAttributes{100};
constexpr MoveCooldown playerMoveCooldown = MoveCooldown{0};

void registerPlayerPrefab(ECSAPI &ecsApi);
