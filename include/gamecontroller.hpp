#pragma once

#include <vector>
#include <memory>

#include <SDL3/SDL.h>

#include "staterunner.hpp"
#include "view/view.hpp"
#include "model/chunk.hpp"
#include "guicontroller.hpp"
#include "model/actor.hpp"
#include "model/character.hpp"

class GameController : public StateRunner
{
    public:
        GameController(View& view, GUIController& guiController);
        GameController(const GameController&) = delete;
        GameController& operator=(const GameController&) = delete;
        GameController(GameController&&) = default;
        GameController& operator=(GameController&&) = default;
        ~GameController() = default;

        int run() override;
        int runCharacterMenu();
        void keyDownListener(const SDL_Keycode key);

        void setRunning(const bool running);
        void showCharacterMenu(const bool visible);

    private:
        bool running;
        bool characterMenu;

        View& view;
        GUIController& guiController;

        Chunk chunk;
        std::vector<std::unique_ptr<Actor>> actors;
        std::vector<Character*> characters;
        Character* player;
};