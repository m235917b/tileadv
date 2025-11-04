#include "systems/rendersystem.hpp"

#include <cmath>

#include "components/actor.hpp"
#include "components/position.hpp"
#include "ecs/ecscontext.hpp"
#include "gui/gui.hpp"
#include "resources/chunkresource.hpp"
#include "resources/playeridresource.hpp"
#include "resources/rendercontextresource.hpp"
#include "resources/textureresource.hpp"
#include "view/view.hpp"

const int cameraMarginX{10};
const int cameraMarginY{10};
const int tileSize{25};
const int leftMargin{10};
const int topMargin{10};

const SDL_FRect getTileCoords(const TileType type) {
  const auto xFactor{static_cast<int>(type) % 100};
  const auto yFactor{static_cast<int>(static_cast<int>(type) / 100)};

  return {xFactor * tileSpriteSize, yFactor * tileSpriteSize, tileSpriteSize,
          tileSpriteSize};
}

const SDL_FRect getActorCoords(const ActorType type) {
  const auto xFactor{static_cast<int>(type)};

  return {xFactor * tileSpriteSize, 0.f, tileSpriteSize, tileSpriteSize};
}

void renderTexture(float posX, float posY, const SDL_FRect &spriteCoords,
                   SDL_Texture *texture, SDL_Renderer *renderer) {
  const SDL_FRect dstRect{posX, posY, tileSize > 0 ? tileSize : spriteCoords.w,
                          tileSize > 0 ? tileSize : spriteCoords.h};
  SDL_RenderTexture(renderer, texture, &spriteCoords, &dstRect);
}

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
        const auto &playerId{
            context.getResourceManager().getResource<PlayerIDResource>()};
        const auto &playerPos{
            context.getStore().getComponent<Position>(playerId->id)};

        const int playerPosX{playerPos->x};
        const int playerPosY{playerPos->y};
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
            float posX{leftMargin + static_cast<float>(x) * tileSize -
                       cameraX * tileSize};
            float posY{topMargin + static_cast<float>(y) * tileSize -
                       cameraY * tileSize};
            const auto tile{getTileAt(*chunk, x, y)};

            const SDL_FRect spriteCoords{getTileCoords(tile.type)};

            renderTexture(posX, posY, spriteCoords, textures->tiles,
                          renderContext->renderer);
          }
        }

        context.getStore().view<Actor, Position>(
            [&](const std::string &, const Actor &actor, const Position &pos) {
              float posX{leftMargin + static_cast<float>(pos.x) * tileSize -
                         cameraX * tileSize};
              float posY{topMargin + static_cast<float>(pos.y) * tileSize -
                         cameraY * tileSize};
              const auto &spriteCoords{getActorCoords(actor.type)};

              if (posX >= leftMargin &&
                  posX + tileSize + leftMargin <= screenWidth &&
                  posY >= topMargin &&
                  posY + tileSize + topMargin <= screenHeight) {
                renderTexture(posX, posY, spriteCoords, textures->actors,
                              renderContext->renderer);
              }
            });

        SDL_RenderPresent(renderer);
      });

  return id;
}
