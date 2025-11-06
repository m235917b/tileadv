#include "chunk/chunk.hpp"
#include "chunk/events.hpp"
#include "chunk/resources.hpp"

void initChunk(ECSContext &ecsContext) {
  initChunkResources(ecsContext);
  initChunkEvents(ecsContext);
}
