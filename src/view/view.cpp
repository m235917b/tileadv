#include <string>
#include <cmath>
#include <memory>

#include <SDL3/SDL.h>

#include "view/view.hpp"
#include "view/ltexture.hpp"
#include "view/tileatlas.hpp"
#include "model/chunk.hpp"

View::View()
    : tileSize(25), cameraX(0), cameraY(0), cameraMarginX(10), cameraMarginY(10), window(nullptr), renderer(nullptr), playerTexture(), worldTiles()
{
    topMargin = (screenHeight - tileSize * static_cast<int>(screenHeight / tileSize)) / 2;
    leftMargin = (screenWidth - tileSize * static_cast<int>(screenWidth / tileSize)) / 2;
}

bool View::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_CreateWindowAndRenderer("TileADV", screenWidth, screenHeight, 0, &window, &renderer) == false)
    {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_SetWindowFullscreen(window, true) == false)
    {
        SDL_Log("Could not set fullscreen mode! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_HideCursor() == false)
    {
        SDL_Log("Could not hide cursor! SDL error: %s\n", SDL_GetError());
        return false;
    }

    /*
    if(SDL_SetRenderVSync(renderer, 1) == false)
    {
        SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
        success = false;
    }
    */

    if(loadTextures() == false)
    {
        SDL_Log("Unable to load media!\n");
        return false;
    }

    return true;
}

int View::destroy()
{
    // asciiGrey.destroy();
    playerTexture.destroy();
    worldTiles.destroy();

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();

    return 0;
}

bool View::loadTextures()
{
    bool success{ true };

    /*if(asciiGrey.loadFromFile("assets/ascii_grey.png", renderer) == false)
    {
        SDL_Log("Unable to load png image!\n");
        success = false;
    }*/

    if(playerTexture.loadFromFile("assets/tiles_char.png", renderer) == false)
    {
        SDL_Log("Unable to load png image!\n");
        success = false;
    }

    if(worldTiles.loadFromFile("assets/tiles_world.png", renderer) == false)
    {
        SDL_Log("Unable to load png image!\n");
        success = false;
    }

    return success;
}

bool View::renderGame(Chunk& chunk, const std::vector<Character*>& characters, const Character* player)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    auto exitCode = drawGame(chunk, characters, player);

    SDL_RenderPresent(renderer);

    return exitCode;
}

bool View::renderMainMenu()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    // Render main menu elements here (e.g., title, buttons)

    SDL_RenderPresent(renderer);

    return true;
}

bool View::renderCharacterMenu(Chunk& chunk, const std::vector<Character*>& characters, const Character* player)
{
    SDL_SetRenderDrawColor(renderer, 0x20, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    auto exitCode1 = drawGame(chunk, characters, player);
    auto exitCode2 = drawCharacterMenu(chunk, characters, player);

    SDL_RenderPresent(renderer);

    return exitCode1 && exitCode2;
}

bool View::drawGame(Chunk& chunk, const std::vector<Character*>& characters, const Character* player)
{
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

bool View::drawCharacterMenu(Chunk& chunk, const std::vector<Character*>& characters, const Character* player)
{
    return true;
}
