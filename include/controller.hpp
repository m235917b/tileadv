#pragma once

#include <vector>
#include <memory>

#include "view/view.hpp"
#include "model/chunk.hpp"
#include "model/actor.hpp"
#include "model/character.hpp"

class Controller
{
    public:
        Controller();

        bool run();
    private:
        View view;
        Chunk chunk;

        Character* player;
        std::vector<std::unique_ptr<Actor>> actors;
        std::vector<Character*> characters;
};