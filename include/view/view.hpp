#pragma once

#include <vector>

#include "model/character.hpp"
#include "utils/ltexture.hpp"
#include "utils/rendercontext.hpp"

class Chunk;

class View {
public:
  View(const RenderContext &renderContext);
  View(const View &) = delete;
  View &operator=(const View &) = delete;
  ~View() = default;

  void drawGame(const Chunk &chunk, const std::vector<Character *> &characters,
                const Character &player);

  bool init();

private:
  const RenderContext &renderContext;

  int topMargin;
  int leftMargin;
  int tileSize;
  int cameraX;
  int cameraY;
  int cameraMarginX;
  int cameraMarginY;

  LTexture playerTexture;
  LTexture worldTiles;

  bool loadTextures();
};