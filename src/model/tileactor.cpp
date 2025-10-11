#include <cmath>

#include "model/chunk.hpp"
#include "model/tileactor.hpp"

TileActor::TileActor(const int posX, const int posY, const TileActorType type)
    : Actor(), posX(posX), posY(posY), speed(0.3f), type(type) {}

void TileActor::move(const float dx, const float dy, const Chunk &chunk) {
  const auto length = std::sqrt(dx * dx + dy * dy);
  const float ndx = (length != 0) ? (dx / length) * speed : 0;
  const float ndy = (length != 0) ? (dy / length) * speed : 0;

  const int nx = static_cast<int>(std::floor(posX + ndx));
  const int ny = static_cast<int>(std::floor(posY + ndy));

  const auto tile{chunk.getTile(nx, ny)};

  if (tile && tile->solid) {
    return;
  }

  posX += ndx;
  posY += ndy;
}

int TileActor::getPosX() const { return static_cast<int>(std::floor(posX)); }

int TileActor::getPosY() const { return static_cast<int>(std::floor(posY)); }

float TileActor::getRealPosX() const { return posX; }

float TileActor::getRealPosY() const { return posY; }

TileActorType TileActor::getType() const { return type; }

void TileActor::setPosX(const int posX) { this->posX = posX; }

void TileActor::setPosY(const int posY) { this->posY = posY; }
