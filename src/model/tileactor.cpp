#include "model/tileactor.hpp"
#include "model/chunk.hpp"

TileActor::TileActor(const int posX, const int posY)
    : Actor(), posX(posX), posY(posY) {}

void TileActor::move(const int dx, const int dy, const Chunk &chunk) {
  const auto tile{chunk.getTile(posX + dx, posY + dy)};

  if (tile && tile->solid) {
    return;
  }

  posX += dx;
  posY += dy;
}

int TileActor::getPosX() const { return posX; }

int TileActor::getPosY() const { return posY; }

void TileActor::setPosX(const int posX) { this->posX = posX; }

void TileActor::setPosY(const int posY) { this->posY = posY; }
