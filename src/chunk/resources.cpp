#include "chunk/resources.hpp"

#include "ecs/ecscontext.hpp"
#include "utils/csvreader.hpp"

void initChunkResource(ECSContext &ecsContext) {
  auto chunk{csvToChunk("world/chunk1.cnk")};
  chunk.id = "chunk1";
  ecsContext.getCommandBuffer().upsertResource(std::move(chunk));
}

Tile getTileAt(const Chunk &chunk, int x, int y) {
  return chunk.worldGrid[y * chunk.sizeX + x];
}

Tile &getTileAt(Chunk &chunk, int x, int y) {
  return chunk.worldGrid[y * chunk.sizeX + x];
}

void setTile(Chunk &chunk, int x, int y, Tile tile) {
  chunk.worldGrid[y * chunk.sizeX + x] = std::move(tile);
}

void initChunkResources(ECSContext &ecsContext) {
  initChunkResource(ecsContext);
}
