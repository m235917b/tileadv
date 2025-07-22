#pragma once

#include "utils/rendercontext.hpp"

struct SDL_Window;
struct SDL_Renderer;

class GraphicsManmager : public RenderContext
{
    public:
        GraphicsManmager();
        GraphicsManmager(const GraphicsManmager&) = delete;
        GraphicsManmager& operator=(const GraphicsManmager&) = delete;
        GraphicsManmager& operator=(GraphicsManmager&&) = delete;
        ~GraphicsManmager() = default;

        bool init();
        int destroy();

        void beginFrame();
        void endFrame();

        const int getScreenWidth() const override;
        const int getScreenHeight() const override;
        SDL_Renderer& getRenderer() const override;

    private:
        int screenWidth;
        int screenHeight;

        SDL_Window* window;
        SDL_Renderer* renderer;
};
