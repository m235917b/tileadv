#include <cmath>

#include "model/chunk.hpp"
#include "model/tileactor.hpp"
#include "utils/math.hpp"

TileActor::TileActor(const int posX, const int posY, const TileActorType type,
                     const int layer)
    : Actor(), posX(posX), posY(posY), alive(true), speedTimer(20), type(type),
      layer(layer) {}

bool TileActor::move(const int dx, const int dy, const Chunk &chunk) {
  if (!speedTimer.intervalElapsed()) {
    return true;
  }

  const int nx = posX + sign(dx);
  const int ny = posY + sign(dy);

  const auto tile{chunk.getTile(nx, ny)};

  if (tile && tile->solid) {
    return false;
  }

  posX = nx;
  posY = ny;

  return true;
}

void TileActor::updateInWorld(
    [[maybe_unused]] const Chunk &chunk,
    [[maybe_unused]] const std::vector<std::unique_ptr<TileActor>>
        &tileActors) {}

int TileActor::getPosX() const { return static_cast<int>(std::floor(posX)); }

int TileActor::getPosY() const { return static_cast<int>(std::floor(posY)); }

float TileActor::getRealPosX() const { return posX; }

float TileActor::getRealPosY() const { return posY; }

TileActorType TileActor::getType() const { return type; }

bool TileActor::isAlive() const { return alive; }

int TileActor::getLayer() const { return layer; }

void TileActor::setPosX(const int posX) { this->posX = posX; }

void TileActor::setPosY(const int posY) { this->posY = posY; }
