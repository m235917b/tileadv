#include <string>
#include <cmath>
#include <memory>

#include <SDL3/SDL.h>

#include "view/view.hpp"
#include "utils/rendercontext.hpp"
#include "utils/ltexture.hpp"
#include "view/tileatlas.hpp"
#include "model/chunk.hpp"

View::View(const RenderContext& renderContext)
    : renderContext(renderContext), tileSize(25), cameraX(0), cameraY(0), cameraMarginX(10), cameraMarginY(10), playerTexture(), worldTiles()
{
    auto screenWidth = this->renderContext.getScreenWidth();
    auto screenHeight{ this->renderContext.getScreenHeight() };

    this->topMargin = (screenHeight - tileSize * static_cast<int>(screenHeight / tileSize)) / 2;
    this->leftMargin = (screenWidth - tileSize * static_cast<int>(screenWidth / tileSize)) / 2;
}

bool View::init()
{
    SDL_Renderer* renderer{ this->renderContext.getRenderer() };

    if(this->playerTexture.loadFromFile("assets/tiles_char.png", renderer) == false)
    {
        SDL_Log("Unable to load png image!\n");

        return false;
    }

    if(this->worldTiles.loadFromFile("assets/tiles_world.png", renderer) == false)
    {
        SDL_Log("Unable to load png image!\n");

        return false;
    }

    return true;
}

const bool View::drawGame(Chunk& chunk, const std::vector<Character*>& characters, const Character* player)
{
    auto screenWidth{ this->renderContext.getScreenWidth() };
    auto screenHeight{ this->renderContext.getScreenHeight() };
    SDL_Renderer* renderer{ this->renderContext.getRenderer() };

    if (player->getPosX() < cameraX + cameraMarginX)
    {
        cameraX = std::max(0, cameraX - 1);
    }

    if (player->getPosX() > cameraX + screenWidth / tileSize - cameraMarginX)
    {
        cameraX = std::min(chunk.getWidth() - screenWidth / tileSize, cameraX + 1);
    }

    if (player->getPosY() < cameraY + cameraMarginY)
    {
        cameraY = std::max(0, cameraY - 1);
    }

    if (player->getPosY() > cameraY + screenHeight / tileSize - cameraMarginY)
    {
        cameraY = std::min(chunk.getHeight() - screenHeight / tileSize, cameraY + 1);
    }

    for (int y = cameraY; y < chunk.getHeight() && y < screenHeight / tileSize + cameraY; ++y)
    {
        for (int x = cameraX; x < chunk.getWidth() && x < screenWidth / tileSize + cameraX; ++x)
        {
            const Tile& tile = chunk.getTile(x, y);
            SDL_FRect spriteCoords = TileAtlas::getSpriteCoords(tile.type);

            float posX = leftMargin + static_cast<float>(x) * tileSize - cameraX * tileSize;
            float posY = topMargin + static_cast<float>(y) * tileSize - cameraY * tileSize;

            worldTiles.render(posX, posY, &spriteCoords, tileSize, tileSize, renderer);
        }
    }

    for (const Character* character : characters)
    {
        float posX = leftMargin + static_cast<float>(character->getPosX()) * tileSize - cameraX * tileSize;
        float posY = topMargin + static_cast<float>(character->getPosY()) * tileSize - cameraY * tileSize;

        if
        (
            posX >= leftMargin &&
            posX + tileSize + leftMargin <= screenWidth &&
            posY >= topMargin &&
            posY + tileSize + topMargin <= screenHeight
        )
        {
            playerTexture.render(posX, posY, nullptr, tileSize, tileSize, renderer);
        }
    }

    return true;
}
