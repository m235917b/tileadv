#include <memory>

#include <SDL3/SDL.h>

#include "controller.hpp"
#include "utils/csvreader.hpp"
#include "character.hpp"

Controller::Controller()
    : view(), chunk(csvToChunk("world/chunk1.wrld")), characters()
{
    std::unique_ptr<Character> player = std::make_unique<Character>(50, 30);
    this->player = player.get();
    characters.push_back(this->player);
    actors.push_back(std::move(player));
}

bool Controller::run()
{
    int exitCode{ 0 };

    if(view.init() == false)
    {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    }
    else
    {
        if(view.loadTextures() == false)
        {
            SDL_Log("Unable to load media!\n");
            exitCode = 2;
        }
        else
        {
            bool quit{ false };

            SDL_Event e;
            SDL_zero(e);
            
            while(quit == false)
            {
                Uint64 startTime{ SDL_GetTicks() };

                while(SDL_PollEvent(&e) == true)
                {
                    if(e.type == SDL_EVENT_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_EVENT_KEY_DOWN)
                    {
                        switch (e.key.key)
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
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            default:
                                break;
                        }
                    }
                }

                view.render(chunk, characters, player);

                Uint64 elapsedTime{ SDL_GetTicks() - startTime };
                Uint64 waitTime{ static_cast<Uint64>(1000 / framerate) - elapsedTime };

                if(waitTime > 0)
                {
                    SDL_Delay(waitTime);
                }
            }
        }
    }

    view.destroy();

    return exitCode;
}
