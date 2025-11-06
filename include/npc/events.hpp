#pragma once

#include <string>

class ECSContext;

struct PopulateChunkNPC {
  std::string chunkId;
};

void initNPCEvents(ECSContext &ecsContext);
