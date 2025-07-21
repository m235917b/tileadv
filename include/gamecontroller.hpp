#pragma once

#include <vector>
#include <memory>

#include <SDL3/SDL.h>

#include "view/view.hpp"
#include "model/chunk.hpp"
#include "model/actor.hpp"
#include "model/character.hpp"

class GameController
{
    public:
        GameController(View* view);
        GameController(const GameController&) = delete;
        GameController& operator=(const GameController&) = delete;
        GameController(GameController&&) = default;
        GameController& operator=(GameController&&) = default;

        int run();
        void keyDownListener(SDL_Keycode& key);

    private:
        View* view;
        Chunk chunk;

        Character* player;
        std::vector<std::unique_ptr<Actor>> actors;
        std::vector<Character*> characters;
};