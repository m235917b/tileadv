#pragma once

#include <vector>
#include <memory>

#include "gui/ltexture.hpp"
#include "character.hpp"

struct SDL_Window;
struct SDL_Renderer;

class Chunk;

constexpr int screenWidth{ 2560 };
constexpr int screenHeight{ 1440 };
constexpr int framerate{ 60 };

class View
{
    public:
        View();

        bool init();
        void destroy();
        bool loadTextures();
        bool render(Chunk& chunk, const std::vector<Character*>& characters);

    private:
        int topMargin;
        int leftMargin;
        int tileSize;

        SDL_Window* window;
        SDL_Renderer* renderer;

        LTexture playerTexture;
        LTexture worldTiles;
};