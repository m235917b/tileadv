#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_FRect;

class LTexture
{
public:
    static constexpr float kOriginalSize = -1.f;

    LTexture();
    LTexture(const LTexture&) = delete;
    LTexture& operator=(const LTexture&) = delete;
    LTexture(LTexture&&) noexcept;
    LTexture& operator=(LTexture&&) noexcept;
    ~LTexture();

    bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

    void destroy();

    void render(float x, float y, SDL_FRect* clip, float width, float height, SDL_Renderer* gRenderer);

    int getWidth();
    int getHeight();
    bool isLoaded();

private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};