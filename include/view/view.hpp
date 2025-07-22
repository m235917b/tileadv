#pragma once

#include <vector>

#include "utils/rendercontext.hpp"
#include "utils/ltexture.hpp"
#include "model/character.hpp"

class Chunk;

class View
{
    public:
        View(const RenderContext& renderContext);
        View(const View&) = delete;
        View& operator=(const View&) = delete;
        ~View() = default;

        const bool drawGame(Chunk& chunk, const std::vector<Character*>& characters, const Character* player);

        bool init();

    private:
        const RenderContext& renderContext;

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