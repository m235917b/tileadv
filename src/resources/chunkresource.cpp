#include "resources/chunkresource.hpp"
#include "ecs/ecscontext.hpp"
#include "utils/csvreader.hpp"

void initChunkResource(ECSContext &ecsContext) {
  auto chunk{csvToChunk("world/chunk1.cnk")};
  const auto chunkRes{std::make_any<Chunk>(std::move(chunk))};
  const auto chunkResAny{UpsertResource{std::move(chunkRes)}};
  ecsContext.getCommandBuffer().enqueue<UpsertResource>(std::move(chunkResAny));
}
