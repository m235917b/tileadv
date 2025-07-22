#include <SDL3/SDL.h>

#include "asc.hpp"

ApplicationStateController::ApplicationStateController() :
    currentState(ApplicationState::MAIN_MENU),
    graphicsManager(),
    view(&graphicsManager),
    guiController(),
    gameController(&view, &guiController)
{

}

int ApplicationStateController::run()
{
    int exitCode{ 0 };

    if (this->graphicsManager.init() == false)
    {
        SDL_Log("Failed to initialize view!\n");
        return 1;
    }

    SDL_Renderer* renderer = this->graphicsManager.getRenderer();

    if (this->view.init() == false)
    {
        SDL_Log("Failed to initialize view!\n");
        return 2;
    }

    if (this->guiController.init(renderer) == false)
    {
        SDL_Log("Failed to initialize view!\n");
        return 1;
    }

    this->guiController.setMainMenuVisible(true);

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

                    switch (this->currentState)
                    {
                        case ApplicationState::MAIN_MENU:
                            switch (e.key.key)
                            {
                                case SDLK_P:
                                    this->currentState = ApplicationState::GAMEPLAY;
                                    this->guiController.setMainMenuVisible(false);
                                    break;

                                default:
                                    break;
                            }
                            break;

                        case ApplicationState::GAMEPLAY:
                            switch (e.key.key)
                            {
                                case SDLK_P:
                                    this->currentState = ApplicationState::MAIN_MENU;
                                    this->guiController.setMainMenuVisible(true);
                                    break;

                                default:
                                    break;
                            }

                            this->gameController.keyDownListener(e.key.key);
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        this->graphicsManager.beginFrame();

        switch (this->currentState)
        {
            case ApplicationState::MAIN_MENU:
                break;

            case ApplicationState::GAMEPLAY:
                gameController.run();
                break;

            default:
                break;
        }

        this->guiController.drawMenus(renderer);

        this->graphicsManager.endFrame();

        Uint64 elapsedTime{ SDL_GetTicks() - startTime };
        Uint64 waitTime{ static_cast<Uint64>(1000 / framerate) - elapsedTime };

        if(waitTime > 0)
        {
            SDL_Delay(waitTime);
        }
    }

    guiController.destroy();
    view.destroy();
    graphicsManager.destroy();

    return exitCode;
}
