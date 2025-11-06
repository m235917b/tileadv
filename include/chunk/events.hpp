#pragma once

class ECSContext;

enum class ChunkBorder { NORTH, WEST, SOUTH, EAST };

struct ChunkBorderEvent {
  ChunkBorder border;
};

void initChunkEvents(ECSContext &context);
