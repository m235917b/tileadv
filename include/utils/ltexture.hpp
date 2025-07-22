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

    const bool loadFromFile(const std::string& path, SDL_Renderer* gRenderer);

    void destroy();

    void render(const float x, const float y, const SDL_FRect* clip, const float width, const float height, SDL_Renderer* gRenderer);

    const int getWidth() const;
    const int getHeight() const;
    const bool isLoaded() const;

private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};