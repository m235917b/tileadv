#pragma once

struct SDL_FRect;

enum class TileType;

enum class TileActorType;

constexpr float tileSpriteSize{25.f};

class TileAtlas {
public:
  static const SDL_FRect getTileCoords(const TileType type);

  static const SDL_FRect getActorCoords(const TileActorType type);
};