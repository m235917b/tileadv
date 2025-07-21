#pragma once

#include <vector>
#include <memory>

#include "view.hpp"
#include "chunk.hpp"
#include "actor.hpp"
#include "character.hpp"

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