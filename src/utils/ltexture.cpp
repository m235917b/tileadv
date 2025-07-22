#include "utils/ltexture.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

LTexture::LTexture():
    mTexture{ nullptr },
    mWidth{ 0 },
    mHeight{ 0 }
{

}

LTexture::LTexture(LTexture&& other) noexcept
    : mTexture(other.mTexture), mWidth(other.mWidth), mHeight(other.mHeight)
{
    other.mTexture = nullptr;
    other.mWidth = 0;
    other.mHeight = 0;
}

LTexture& LTexture::operator=(LTexture&& other) noexcept
{
    if (this != &other) {
        destroy();
        mTexture = other.mTexture;
        mWidth = other.mWidth;
        mHeight = other.mHeight;
        other.mTexture = nullptr;
        other.mWidth = 0;
        other.mHeight = 0;
    }
    return *this;
}

LTexture::~LTexture()
{
    destroy();
}

bool LTexture::loadFromFile( std::string path, SDL_Renderer* gRenderer )
{
    destroy();

    if( SDL_Surface* loadedSurface = IMG_Load( path.c_str() ); loadedSurface == nullptr )
    {
        SDL_Log( "Unable to load image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        if( SDL_SetSurfaceColorKey( loadedSurface, true, SDL_MapSurfaceRGB( loadedSurface, 0xFF, 0x00, 0xFF ) ) == false )
        {
            SDL_Log( "Unable to color key! SDL error: %s", SDL_GetError() );
        }
        else
        {
            if( mTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface ); mTexture == nullptr )
            {
                SDL_Log( "Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError() );
            }
            else
            {
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }
        }

        SDL_DestroySurface( loadedSurface );
    }

    return mTexture != nullptr;
}

void LTexture::destroy()
{
    SDL_DestroyTexture( mTexture );
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::render( float x, float y, SDL_FRect* clip, float width, float height, SDL_Renderer* gRenderer )
{
    SDL_FRect dstRect{ x, y, static_cast<float>( mWidth ), static_cast<float>( mHeight ) };

    if( clip != nullptr )
    {
        dstRect.w = clip->w;
        dstRect.h = clip->h;
    }

    if( width > 0 )
    {
        dstRect.w = width;
    }
    if( height > 0 )
    {
        dstRect.h = height;
    }

    SDL_RenderTexture( gRenderer, mTexture, clip, &dstRect );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool LTexture::isLoaded()
{
    return mTexture != nullptr;
}
