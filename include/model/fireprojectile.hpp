#pragma once

#include <vector>

#include "model/tileactor.hpp"

class FireProjectile : public TileActor {
public:
  FireProjectile(const std::vector<std::pair<int, int>> &path);
  ~FireProjectile() = default;

  void updateInWorld(
      const Chunk &chunk,
      const std::vector<std::unique_ptr<TileActor>> &tileActors) override;

private:
  const std::vector<std::pair<int, int>> path;
  size_t currentIndex;
};
