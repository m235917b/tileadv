#pragma once

#include <string>

struct Chunk;

Chunk csvToChunk(const std::string& filename);
