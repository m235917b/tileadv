#include "model/fireprojectile.hpp"
#include "model/chunk.hpp"

FireProjectile::FireProjectile(const std::vector<std::pair<int, int>> &path)
    : TileActor(path.front().first, path.front().second,
                TileActorType::FIREBALL, 4),
      path(path), currentIndex(1) {}

void FireProjectile::updateInWorld(
    const Chunk &chunk,
    const std::vector<std::unique_ptr<TileActor>> &tileActors) {
  if (!speedTimer.intervalElapsed()) {
    return;
  }

  if (currentIndex < path.size()) {
    const int nx = path[currentIndex].first;
    const int ny = path[currentIndex].second;

    const auto tile{chunk.getTile(nx, ny)};

    if (tile && tile->solid) {
      alive = false;
      return;
    }

    for (const auto &tileActor : tileActors) {
      if (tileActor.get() != this && tileActor->getPosX() == nx &&
          tileActor->getPosY() == ny && tileActor->isAlive()) {
        alive = false;
        return;
      }
    }

    posX = nx;
    posY = ny;

    currentIndex++;
  } else {
    alive = false;
  }
}
