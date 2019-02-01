#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include <stdexcept>

class MainLoop
{
private:
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;

    void initSdl()
    {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(
                    "Chip 8 emulator.",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    640,
                    480,
                    SDL_WINDOW_RESIZABLE
                    );

        if (window == nullptr) {
            throw std::runtime_error(SDL_GetError());
        }

        renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        if ( renderer == nullptr )
        {
            throw std::runtime_error(SDL_GetError());
        }

        SDL_SetRenderDrawColor(renderer, 0xCA, 0xDC, 0x9F, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

    }

    void render()
    {
        SDL_SetRenderDrawColor(renderer, 0xCA, 0xDC, 0x9F, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0x30, 0x62, 0x30, 255);
        SDL_Rect r;

        r.x = 50;
        r.y = 50;
        r.w = 50;
        r.h = 50;
        SDL_RenderFillRect( renderer, &r );
        SDL_RenderPresent(renderer);
    }

    void processEvents()
    {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_QUIT )
                running = false;
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
//                case SDLK_RIGHT:
//                    ++playerPos.x;
//                    break;
//                case SDLK_LEFT:
//                    --playerPos.x;
//                    break;
//                    // Remeber 0,0 in SDL is left-top. So when the user pressus down, the y need to increase
//                case SDLK_DOWN:
//                    ++playerPos.y;
//                    break;
//                case SDLK_UP:
//                    --playerPos.y;
//                    break;
                default :
                    break;
                }
            }
        }
    }

public:
    MainLoop()
    {
        initSdl();
        running = false;
    }
    ~MainLoop()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void run()
    {
        running = true;
        while(running)
        {
            render();
            processEvents();
            SDL_Delay(10);
        }
    }
};

#endif // MAINLOOP_H
