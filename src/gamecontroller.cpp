#include <memory>

#include <SDL3/SDL.h>

#include "gamecontroller.hpp"
#include "utils/csvreader.hpp"
#include "model/character.hpp"

GameController::GameController(View* view)
    : view(view), chunk(csvToChunk("world/chunk1.wrld")), actors(), characters()
{
    std::unique_ptr<Character> player = std::make_unique<Character>(50, 30);
    this->player = player.get();
    characters.push_back(this->player);
    actors.push_back(std::move(player));
}

int GameController::run()
{
    view->renderGame(chunk, characters, player);

    return 0;
}

void GameController::keyDownListener(SDL_Keycode& key)
{
    switch (key)
    {
        case SDLK_UP:
            player->move(0, -1, chunk);
            break;
        case SDLK_DOWN:
            player->move(0, 1, chunk);
            break;
        case SDLK_LEFT:
            player->move(-1, 0, chunk);
            break;
        case SDLK_RIGHT:
            player->move(1, 0, chunk);
            break;
        default:
            break;
    }
}
