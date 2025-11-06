#pragma once

#include <string>
#include <vector>

class ECSContext;

enum class TileType {
  EMPTY = 0,
  GRASS = 1,
  SAND = 2,
  COBBLESTONE = 3,
  SWAMP = 4,
  BRICKWALL = 100,
  WOODWALL = 101,
  STONEWALL = 102
};

struct Tile {
  TileType type;
  bool solid{false};
};

struct Chunk {
  int sizeX;
  int sizeY;
  std::vector<Tile> worldGrid;
  std::string up;
  std::string right;
  std::string down;
  std::string left;
  std::string id;
};

Tile getTileAt(const Chunk &chunk, int x, int y);

Tile &getTileAt(Chunk &chunk, int x, int y);

void setTile(Chunk &chunk, int x, int y, Tile tile);

void initChunkResources(ECSContext &ecsContext);
