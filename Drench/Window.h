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
        //SDL_UpdateWindowSurface(SDLWindow);
        SDL_RenderPresent(SDLRenderer);
    }

    void SetBackgroundColor(int R, int G, int B) {
        bgRed = R;
        bgGreen = G;
        bgBlue = B;
    }

    void SetTitle(std::string NewTitle) {
        // https://wiki.libsdl.org/SDL_SetWindowTitle
        SDL_SetWindowTitle(
            SDLWindow, NewTitle.c_str()
        );
    }

    void ChangeWindowSize(int Amount) {
        // https://wiki.libsdl.org/SDL_SetWindowSize
        SDL_SetWindowSize(
            SDLWindow,
            windowWidth += Amount,
            windowHeight += Amount
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

    int MoveRelative(int x, int y) {
        // https://wiki.libsdl.org/SDL_GetWindowPosition
        int CurrentX; int CurrentY;
        SDL_GetWindowPosition(
            SDLWindow, &CurrentX, &CurrentY
        );

        // https://wiki.libsdl.org/SDL_SetWindowPosition
        SDL_SetWindowPosition(
            SDLWindow, CurrentX + x, CurrentY + y
        );
    }

    void GrabMouse() {
        // https://wiki.libsdl.org/SDL_SetWindowMouseGrab
        SDL_SetWindowMouseGrab(SDLWindow, SDL_TRUE);
    }

    void FreeMouse() {
        // https://wiki.libsdl.org/SDL_SetWindowMouseGrab
        SDL_SetWindowMouseGrab(SDLWindow, SDL_FALSE);
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