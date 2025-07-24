#include <SDL3/SDL.h>

#include "asc.hpp"

ApplicationStateController::ApplicationStateController() :
    currentState(ApplicationState::MAIN_MENU),
    graphicsManager(),
    view(graphicsManager),
    guiController(graphicsManager),
    gameController(view, guiController)
{

}

int ApplicationStateController::run()
{
    int exitCode{ 0 };

    if (graphicsManager.init() == false)
    {
        SDL_Log("Failed to initialize view!\n");
        return 1;
    }

    if (view.init() == false)
    {
        SDL_Log("Failed to initialize view!\n");
        return 2;
    }

    if (guiController.init() == false)
    {
        SDL_Log("Failed to initialize view!\n");
        return 3;
    }

    bool run{ true };

    guiController.addKeyListener("2_play", SDLK_RETURN, [this] () {
        currentState = ApplicationState::GAMEPLAY;
        guiController.setMainMenuVisible(false);
        gameController.setRunning(true);
    });

    guiController.addKeyListener("3_exit", SDLK_RETURN, [&run] () {
        run = false;
    });

    guiController.setMainMenuVisible(true);

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
                    switch (currentState)
                    {
                        case ApplicationState::MAIN_MENU:
                            switch (e.key.key)
                            {
                                case SDLK_ESCAPE:
                                    currentState = ApplicationState::GAMEPLAY;
                                    guiController.setMainMenuVisible(false);
                                    gameController.setRunning(true);
                                    break;

                                default:
                                    break;
                            }
                            break;

                        case ApplicationState::GAMEPLAY:
                            switch (e.key.key)
                            {
                                case SDLK_ESCAPE:
                                    currentState = ApplicationState::MAIN_MENU;
                                    guiController.setMainMenuVisible(true);
                                    gameController.setRunning(false);
                                    break;

                                default:
                                    break;
                            }

                            break;

                        default:
                            break;
                    }

                    gameController.keyDownListener(e.key.key);
                    guiController.keyDownListener(e.key.key);

                    break;

                default:
                    break;
            }
        }

        graphicsManager.beginFrame();

        gameController.run();
        guiController.run();

        graphicsManager.endFrame();

        Uint64 elapsedTime{ SDL_GetTicks() - startTime };
        Uint64 waitTime{ static_cast<Uint64>(1000 / framerate) - elapsedTime };

        if(waitTime > 0)
        {
            SDL_Delay(waitTime);
        }
    }

    graphicsManager.destroy();

    return exitCode;
}
