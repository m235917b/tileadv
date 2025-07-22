#pragma once

struct SDL_FRect;

constexpr float asciiWidth{ 24.f };
constexpr float asciiHeight{ 50.f };

class AsciiAtlas
{
    public:
        static SDL_FRect getSpriteCoords(char asciiChar);
};
