#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include <stdexcept>

#include "chipeight.h"

class MainLoop
{
private:
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;

    ChipEight chipeight;
    int windowHeight = 480;
    int windowWidth = 640;

    void initSdl()
    {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(
                    "Chip 8 emulator.",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    windowWidth,
                    windowHeight,
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
        const int CHIP_SCREEN_WIDTH = 64;
        const int CHIP_SCREEN_HEIGHT = 32;
        SDL_SetRenderDrawColor(renderer, 0xCA, 0xDC, 0x9F, 255);
        SDL_RenderClear(renderer);
        for(int y = 0; y < CHIP_SCREEN_HEIGHT; y++)
        {
            for(int x = 0; x < CHIP_SCREEN_WIDTH; x++)
            {
                if(chipeight.getPixel(x, y) == 1)
                {
                    SDL_Rect r;
                    int width_coef = (windowWidth-CHIP_SCREEN_WIDTH)/CHIP_SCREEN_WIDTH;
                    int height_coef = (windowHeight-CHIP_SCREEN_HEIGHT)/CHIP_SCREEN_HEIGHT;
                    r.x = x*width_coef;
                    r.y = y*height_coef;
                    r.w = width_coef;
                    r.h = height_coef;
                    SDL_SetRenderDrawColor(renderer, 0x30, 0x62, 0x30, 255);
                    SDL_RenderFillRect( renderer, &r );
                }
            }
            std::cout << "\n";
        }
        chipeight.drawFlag = false;
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
            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                }
            }
        }
    }

public:
    MainLoop(int argc, char *argv[])
    {
        initSdl();
        chipeight.init();
        chipeight.load("./spaceInvaders.ch8");
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
            if(chipeight.drawFlag)
                render();
            processEvents();
            chipeight.cycle();
            SDL_Delay(2);
        }
    }
};

#endif // MAINLOOP_H
