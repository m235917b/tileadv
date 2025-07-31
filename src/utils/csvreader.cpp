#include <fstream>
#include <sstream>
#include <vector>

#include "model/chunk.hpp"
#include "model/tile.hpp"
#include "utils/csvreader.hpp"

Chunk csvToChunk(const std::string &filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + filename);
  }

  std::string line;

  std::getline(file, line);
  std::stringstream ss(line);
  std::string value;
  std::getline(ss, value, ',');
  const int sizex{std::stoi(value)};
  std::getline(ss, value, ',');
  const int sizey{std::stoi(value)};
  std::string up;
  std::getline(ss, up, ',');
  std::string right;
  std::getline(ss, right, ',');
  std::string down;
  std::getline(ss, down, ',');
  std::string left;
  std::getline(ss, left, ',');

  Chunk chunk(sizex, sizey);
  chunk.setNeighbors(up, right, down, left);

  int posX{0};
  int posY{0};

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string value;
    while (std::getline(ss, value, ',')) {
      const Tile tile{TileFactory::fromInt(std::stoi(value))};
      chunk.setTile(posX, posY, tile);
      posX++;
    }
    posY++;
    posX = 0;
  }

  return chunk;
}
