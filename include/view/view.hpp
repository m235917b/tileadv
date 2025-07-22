#pragma once

#include <vector>
#include <memory>

#include "utils/rendercontext.hpp"
#include "utils/ltexture.hpp"
#include "model/character.hpp"

class Chunk;

class View
{
    public:
        View(RenderContext* renderContext);
        View(const View&) = delete;
        View& operator=(const View&) = delete;
        ~View() = default;

        bool drawGame(Chunk& chunk, const std::vector<Character*>& characters, const Character* player);

        bool init();
        void destroy();

    private:
        RenderContext* renderContext;

        int topMargin;
        int leftMargin;
        int tileSize;
        int cameraX;
        int cameraY;
        int cameraMarginX;
        int cameraMarginY;

        LTexture playerTexture;
        LTexture worldTiles;

        bool loadTextures();
};