#include "model/tileactor.hpp"
#include "model/chunk.hpp"

TileActor::TileActor(const int posX, const int posY)
    : Actor(), posX(posX), posY(posY) {}

void TileActor::move(const int dx, const int dy, const Chunk &chunk) {
  if (posX + dx < 0 || posX + dx >= chunk.getWidth() || posY + dy < 0 ||
      posY + dy >= chunk.getHeight()) {
    return;
  }

  if (chunk.getTile(posX + dx, posY + dy).solid) {
    return;
  }

  posX += dx;
  posY += dy;
}

int TileActor::getPosX() const { return posX; }

int TileActor::getPosY() const { return posY; }

void TileActor::setPosX(const int posX) { this->posX = posX; }

void TileActor::setPosY(const int posY) { this->posY = posY; }
