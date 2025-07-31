#pragma once

#include <string>
#include <vector>


#include "model/tile.hpp"

class Chunk {
public:
  Chunk(const int sizex, const int sizey);

  bool setTile(const int posx, const int posy, const Tile tile);

  const Tile &getTile(const int posx, const int posy) const;
  int getWidth() const;
  int getHeight() const;

private:
  int sizex;
  int sizey;
  std::vector<Tile> worldGrid;
  std::string up;
  std::string right;
  std::string left;
  std::string down;
};
