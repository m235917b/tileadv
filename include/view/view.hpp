#pragma once

#include <vector>
#include <memory>

#include "view/ltexture.hpp"
#include "model/character.hpp"

struct SDL_Window;
struct SDL_Renderer;

class Chunk;
class GUIMenu;
class GUIContainer;
class GUIElement;

constexpr int screenWidth{ 2560 };
constexpr int screenHeight{ 1440 };
constexpr int framerate{ 60 };

class View
{
    public:
        View();
        View(const View&) = delete;
        View& operator=(const View&) = delete;
        ~View() = default;

        bool renderGame(Chunk& chunk, const std::vector<Character*>& characters, const Character* player);
        bool renderMainMenu(const GUIMenu& mainMenu);
        bool renderCharacterMenu(const GUIMenu& characterMenu, Chunk& chunk, const std::vector<Character*>& characters, const Character* player);

        bool init();
        int destroy();

    private:
        int topMargin;
        int leftMargin;
        int tileSize;
        int cameraX;
        int cameraY;
        int cameraMarginX;
        int cameraMarginY;

        SDL_Window* window;
        SDL_Renderer* renderer;

        LTexture asciiGrey;
        LTexture playerTexture;
        LTexture worldTiles;

        bool loadTextures();

        bool drawGame(Chunk& chunk, const std::vector<Character*>& characters, const Character* player);
        bool drawCharacterMenu(Chunk& chunk, const std::vector<Character*>& characters, const Character* player);

        bool drawText(int posX, int posY, float size, const std::string& text);
        bool drawMenu(const GUIMenu& menu);
        bool drawVerticalLayout(int posX, int posY, const GUIContainer& container);
        bool drawTextElement(int posX, int posY, const GUIElement& element);
};