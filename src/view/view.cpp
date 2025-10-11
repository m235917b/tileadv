#include <cmath>
#include <string>

#include <SDL3/SDL.h>

#include "model/chunk.hpp"
#include "view/tileatlas.hpp"
#include "view/view.hpp"

View::View(const RenderContext &renderContext)
    : renderContext(renderContext), tileSize(25), cameraX(0), cameraY(0),
      cameraMarginX(10), cameraMarginY(10), characterTexture(), worldTiles() {
  auto screenWidth{renderContext.getScreenWidth()};
  auto screenHeight{renderContext.getScreenHeight()};

  topMargin =
      (screenHeight - tileSize * static_cast<int>(screenHeight / tileSize)) / 2;
  leftMargin =
      (screenWidth - tileSize * static_cast<int>(screenWidth / tileSize)) / 2;
}

bool View::init() {
  SDL_Renderer &renderer{renderContext.getRenderer()};

  if (characterTexture.loadFromFile("assets/tiles_char.png", renderer) ==
      false) {
    SDL_Log("Unable to load png image!\n");

    return false;
  }

  if (worldTiles.loadFromFile("assets/tiles_world.png", renderer) == false) {
    SDL_Log("Unable to load png image!\n");

    return false;
  }

  return true;
}

void View::drawGame(const Chunk &chunk, std::vector<TileActor *> &tileActors,
                    const Character &player) {
  auto screenWidth{renderContext.getScreenWidth()};
  auto screenHeight{renderContext.getScreenHeight()};
  SDL_Renderer &renderer{renderContext.getRenderer()};

  if (player.getPosX() < cameraX + cameraMarginX) {
    cameraX = std::max(0, player.getPosX() - cameraMarginX);
  }

  if (player.getPosX() > cameraX + screenWidth / tileSize - cameraMarginX) {
    cameraX =
        std::min(chunk.getWidth() - screenWidth / tileSize,
                 player.getPosX() - screenWidth / tileSize + cameraMarginX);
  }

  if (player.getPosY() < cameraY + cameraMarginY) {
    cameraY = std::max(0, player.getPosY() - cameraMarginY);
  }

  if (player.getPosY() > cameraY + screenHeight / tileSize - cameraMarginY) {
    cameraY =
        std::min(chunk.getHeight() - screenHeight / tileSize,
                 player.getPosY() - screenHeight / tileSize + cameraMarginY);
  }

  for (int y{cameraY};
       y < chunk.getHeight() && y < screenHeight / tileSize + cameraY; ++y) {
    for (int x{cameraX};
         x < chunk.getWidth() && x < screenWidth / tileSize + cameraX; ++x) {
      const auto tile{chunk.getTile(x, y)};
      if (tile) {
        SDL_FRect spriteCoords{TileAtlas::getTileCoords(tile->type)};

        float posX{leftMargin + static_cast<float>(x) * tileSize -
                   cameraX * tileSize};
        float posY{topMargin + static_cast<float>(y) * tileSize -
                   cameraY * tileSize};

        worldTiles.render(posX, posY, &spriteCoords, tileSize, tileSize,
                          renderer);
      }
    }
  }

  for (const auto &actor : tileActors) {
    float posX{leftMargin + static_cast<float>(actor->getPosX()) * tileSize -
               cameraX * tileSize};
    float posY{topMargin + static_cast<float>(actor->getPosY()) * tileSize -
               cameraY * tileSize};

    const auto spriteCoords{TileAtlas::getActorCoords(actor->getType())};

    if (posX >= leftMargin && posX + tileSize + leftMargin <= screenWidth &&
        posY >= topMargin && posY + tileSize + topMargin <= screenHeight) {
      characterTexture.render(posX, posY, &spriteCoords, tileSize, tileSize,
                              renderer);
    }
  }
}
