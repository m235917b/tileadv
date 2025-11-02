#include "systems/rendersystem.hpp"

#include <cmath>

#include "ecs/ecscontext.hpp"
#include "gui/guicontext.hpp"
#include "resources/chunkresource.hpp"
#include "resources/rendercontextresource.hpp"
#include "resources/textureresource.hpp"
#include "view/view.hpp"

const int cameraMarginX{10};
const int cameraMarginY{10};
const int playerPosX{100};
const int playerPosY{200};
const int tileSize{25};
const int leftMargin{10};
const int topMargin{10};

const SDL_FRect getTileCoords(const TileType type) {
  const auto xFactor{static_cast<int>(type) % 100};
  const auto yFactor{static_cast<int>(static_cast<int>(type) / 100)};

  return {xFactor * tileSpriteSize, yFactor * tileSpriteSize, tileSpriteSize,
          tileSpriteSize};
}

/*const SDL_FRect getActorCoords(const TileActorType type) {
  const auto xFactor{static_cast<int>(type)};

  return {xFactor * tileSpriteSize, 0.f, tileSpriteSize, tileSpriteSize};
}*/

std::string registerRenderSystem(const std::string &phase,
                                 ECSContext &context) {
  const std::string id{"render"};

  context.getScheduler().registerSystem(
      phase, id, [](ECSContext &context, const float) {
        const auto *renderContext{context.getResourceManager()
                                      .getResource<RenderContextResource>()
                                      ->context};
        const auto &renderer{renderContext->renderer};
        auto screenWidth{renderContext->screenWidth};
        auto screenHeight{renderContext->screenHeight};
        const auto &chunk{context.getResourceManager().getResource<Chunk>()};
        const auto &textures{
            context.getResourceManager().getResource<TextureResource>()};

        int cameraX{0};
        int cameraY{0};

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        if (playerPosX < cameraX + cameraMarginX) {
          cameraX = std::max(0, playerPosX - cameraMarginX);
        }

        if (playerPosX > cameraX + screenWidth / tileSize - cameraMarginX) {
          cameraX =
              std::min(chunk->sizeX - screenWidth / tileSize,
                       playerPosX - screenWidth / tileSize + cameraMarginX);
        }

        if (playerPosY < cameraY + cameraMarginY) {
          cameraY = std::max(0, playerPosY - cameraMarginY);
        }

        if (playerPosY > cameraY + screenHeight / tileSize - cameraMarginY) {
          cameraY =
              std::min(chunk->sizeY - screenHeight / tileSize,
                       playerPosY - screenHeight / tileSize + cameraMarginY);
        }

        for (int y{cameraY};
             y < chunk->sizeX && y < screenHeight / tileSize + cameraY; ++y) {
          for (int x{cameraX};
               x < chunk->sizeY && x < screenWidth / tileSize + cameraX; ++x) {
            const auto tile{chunk->worldGrid.at(y * chunk->sizeX + x)};

            const SDL_FRect spriteCoords{getTileCoords(tile.type)};

            float posX{leftMargin + static_cast<float>(x) * tileSize -
                       cameraX * tileSize};
            float posY{topMargin + static_cast<float>(y) * tileSize -
                       cameraY * tileSize};

            const SDL_FRect dstRect{posX, posY,
                                    tileSize > 0 ? tileSize : spriteCoords.w,
                                    tileSize > 0 ? tileSize : spriteCoords.h};

            SDL_RenderTexture(renderer, textures->tiles, &spriteCoords,
                              &dstRect);
          }
        }

        SDL_RenderPresent(renderer);
      });

  return id;
}
