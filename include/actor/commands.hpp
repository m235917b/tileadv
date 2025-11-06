#pragma once

#include <string>

class ECSContext;

struct PopulateChunk {
  std::string chunkId;
};

void initActorCommands(ECSContext &ecsContext);
