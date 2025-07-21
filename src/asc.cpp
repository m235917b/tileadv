#include <SDL3/SDL.h>

#include "asc.hpp"

ApplicationStateController::ApplicationStateController() :
    currentState{ApplicationState::MAIN_MENU},
    view(),
    guiController(&view),
    gameController(&view, &guiController)
{
    guiController.initMainMenus();
}

int ApplicationStateController::run()
{
    int exitCode{ 0 };

    if (view.init() == false)
    {
        SDL_Log("Failed to initialize view!\n");
        return 1;
    }

    bool run{ true };

    SDL_Event e;
    SDL_zero(e);
            
    while(run)
    {
        Uint64 startTime{ SDL_GetTicks() };

        while(SDL_PollEvent(&e) == true)
        {
            switch (e.type)
            {
                case SDL_EVENT_QUIT:
                    run = false;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    switch (e.key.key)
                    {
                        case SDLK_ESCAPE:
                            run = false;
                            break;

                        default:
                            break;
                    }

                    switch (currentState)
                    {
                        case ApplicationState::MAIN_MENU:
                            switch (e.key.key)
                            {
                                case SDLK_P:
                                    currentState = ApplicationState::GAMEPLAY;
                                    break;

                                default:
                                    break;
                            }
                            break;

                        case ApplicationState::GAMEPLAY:
                            switch (e.key.key)
                            {
                                case SDLK_P:
                                    currentState = ApplicationState::MAIN_MENU;
                                    break;

                                default:
                                    break;
                            }

                            gameController.keyDownListener(e.key.key);
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        switch (currentState)
        {
            case ApplicationState::MAIN_MENU:
                guiController.showMainMenu();
                break;

            case ApplicationState::GAMEPLAY:
                gameController.run();
                break;

            default:
                break;
        }

        Uint64 elapsedTime{ SDL_GetTicks() - startTime };
        Uint64 waitTime{ static_cast<Uint64>(1000 / framerate) - elapsedTime };

        if(waitTime > 0)
        {
            SDL_Delay(waitTime);
        }
    }

    view.destroy();

    return exitCode;
}
