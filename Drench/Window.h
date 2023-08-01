// Thanks to www.studyplan.dev

#pragma once
#include <SDL.h>
#include <string>

class Window {
public:
    Window() {
        SDL_Init(SDL_INIT_VIDEO);

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDLWindow = SDL_CreateWindow(
            "Drench",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowWidth, windowHeight, 0
        );

        if (SDLWindow == NULL)
        {
            std::cout << "SDLWindow could not be created for window " << this << "SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
        }

        SDLRenderer = SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED);

        if (SDLRenderer == NULL)
        {
            std::cout << "SDLRenderer could not be created for window " << this << "\nSDL Error: " << SDL_GetError() <<  std::endl;
            return;
        }
    }

    void RenderFrame() {
        SDL_RenderPresent(SDLRenderer);
    }

    void SetBackgroundColor(int R, int G, int B) {
        bgRed = R;
        bgGreen = G;
        bgBlue = B;
    }

    void SetTitle(std::string NewTitle) {
        SDL_SetWindowTitle(
            SDLWindow, NewTitle.c_str()
        );
    }

    [[nodiscard]]
    int GetWindowWidth() const {
        return windowWidth;
    }

    [[nodiscard]]
    int GetWindowHeight() const {
        return windowHeight;
    }

    bool IsWindowValid()
    {
        return SDLWindow == nullptr;
    }

    void DestroyWindow()
    {
        SDL_DestroyRenderer(SDLRenderer);
        SDL_DestroyWindow(SDLWindow);

        SDLWindow = nullptr;
        SDLRenderer = nullptr;
    }

    SDL_Renderer* GetRenderer()
    {
        return SDLRenderer;
    }

private:
    SDL_Window* SDLWindow{ nullptr };
    SDL_Renderer* SDLRenderer{ nullptr };
    int windowWidth{ 900 };
    int windowHeight{ 900 };

    int bgRed{ 40 };
    int bgGreen{ 40 };
    int bgBlue{ 40 };
};