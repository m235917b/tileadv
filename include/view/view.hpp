#pragma once

#include <memory>
#include <vector>

#include "model/character.hpp"
#include "utils/ltexture.hpp"
#include "utils/rendercontext.hpp"

class Chunk;
class TileActor;

class View {
public:
  View(const RenderContext &renderContext);
  View(const View &) = delete;
  View &operator=(const View &) = delete;
  ~View() = default;

  void drawGame(const Chunk &chunk,
                std::vector<std::unique_ptr<TileActor>> &tileActors,
                const Character &player);

  bool init();

  std::pair<int, int> getTileFromPixel(const int x, const int y) const;

private:
  const RenderContext &renderContext;

  int topMargin;
  int leftMargin;
  int tileSize;
  int cameraX;
  int cameraY;
  int cameraMarginX;
  int cameraMarginY;

  LTexture characterTexture;
  LTexture worldTiles;

  bool loadTextures();
};