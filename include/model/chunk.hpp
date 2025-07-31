#pragma once

#include <string>
#include <vector>

#include "model/tile.hpp"

class Chunk {
public:
  Chunk(const int sizex, const int sizey);

  bool setTile(const int posX, const int posY, const Tile tile);
  void setNeighbors(std::string up, std::string right, std::string down,
                    std::string left);

  const Tile *getTile(const int posX, const int posY) const;
  int getWidth() const;
  int getHeight() const;
  std::string getUp() const;
  std::string getRight() const;
  std::string getDown() const;
  std::string getLeft() const;

private:
  int sizex;
  int sizey;
  std::vector<Tile> worldGrid;
  std::string up;
  std::string right;
  std::string down;
  std::string left;
};
