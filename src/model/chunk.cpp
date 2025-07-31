#include "model/chunk.hpp"

Chunk::Chunk(int sizex, int sizey)
    : sizex{sizex}, sizey{sizey}, worldGrid(), up(), right(), down(), left() {
  worldGrid = std::vector<Tile>(sizex * sizey, {TileType::EMPTY});
}

bool Chunk::setTile(const int posX, const int posY, const Tile tile) {
  if (posX < 0 || posX >= sizex || posY < 0 || posY >= sizey) {
    return false;
  }

  worldGrid[posY * sizex + posX] = tile;

  return true;
}

void Chunk::setNeighbors(std::string up, std::string right, std::string down,
                         std::string left) {
  this->up = up;
  this->right = right;
  this->down = down;
  this->left = left;
}

const Tile &Chunk::getTile(const int posX, const int posY) const {
  return worldGrid[posY * sizex + posX];
}

int Chunk::getWidth() const { return sizex; }

int Chunk::getHeight() const { return sizey; }

std::string Chunk::getUp() const { return up; }

std::string Chunk::getRight() const { return right; }

std::string Chunk::getDown() const { return down; }

std::string Chunk::getLeft() const { return left; }
