#pragma once

struct SDL_Window;
struct SDL_Renderer;

class GraphicsManmager
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

        const int getScreenWidth() const;
        const int getScreenHight() const;
        SDL_Renderer* getRenderer() const;

    private:
        int screenWidth;
        int screenHeight;

        SDL_Window* window;
        SDL_Renderer* renderer;
};
