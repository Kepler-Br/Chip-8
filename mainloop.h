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
                    int width_coef = (windowWidth-CHIP_SCREEN_WIDTH)/CHIP_SCREEN_WIDTH+1;
                    int height_coef = (windowHeight-CHIP_SCREEN_HEIGHT)/CHIP_SCREEN_HEIGHT+1;
                    r.x = x*width_coef;
                    r.y = y*height_coef;
                    r.w = width_coef;
                    r.h = height_coef;
                    SDL_SetRenderDrawColor(renderer, 0x30, 0x62, 0x30, 255);
                    SDL_RenderFillRect( renderer, &r );
                }
            }
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
                case SDLK_1:
                    chipeight.pressKey(0x1);
                    break;
                case SDLK_2:
                    chipeight.pressKey(0x2);
                    break;
                case SDLK_3:
                    chipeight.pressKey(0x3);
                    break;
                case SDLK_4:
                    chipeight.pressKey(0xC);
                    break;

                case SDLK_q:
                    chipeight.pressKey(0x4);
                    break;
                case SDLK_w:
                    chipeight.pressKey(0x5);
                    break;
                case SDLK_e:
                    chipeight.pressKey(0x6);
                    break;
                case SDLK_r:
                    chipeight.pressKey(0xD);
                    break;

                case SDLK_a:
                    chipeight.pressKey(0x7);
                    break;
                case SDLK_s:
                    chipeight.pressKey(0x8);
                    break;
                case SDLK_d:
                    chipeight.pressKey(0x9);
                    break;
                case SDLK_f:
                    chipeight.pressKey(0xE);
                    break;

                case SDLK_z:
                    chipeight.pressKey(0xA);
                    break;
                case SDLK_x:
                    chipeight.pressKey(0x0);
                    break;
                case SDLK_c:
                    chipeight.pressKey(0xB);
                    break;
                case SDLK_v:
                    chipeight.pressKey(0xF);
                    break;
                default :
                    break;
                }
            }
            else if ( event.type == SDL_KEYUP )
            {
                switch ( event.key.keysym.sym )
                {
                case SDLK_1:
                    chipeight.releaseKey(0x1);
                    break;
                case SDLK_2:
                    chipeight.releaseKey(0x2);
                    break;
                case SDLK_3:
                    chipeight.releaseKey(0x3);
                    break;
                case SDLK_4:
                    chipeight.releaseKey(0xC);
                    break;

                case SDLK_q:
                    chipeight.releaseKey(0x4);
                    break;
                case SDLK_w:
                    chipeight.releaseKey(0x5);
                    break;
                case SDLK_e:
                    chipeight.releaseKey(0x6);
                    break;
                case SDLK_r:
                    chipeight.releaseKey(0xD);
                    break;

                case SDLK_a:
                    chipeight.releaseKey(0x7);
                    break;
                case SDLK_s:
                    chipeight.releaseKey(0x8);
                    break;
                case SDLK_d:
                    chipeight.releaseKey(0x9);
                    break;
                case SDLK_f:
                    chipeight.releaseKey(0xE);
                    break;

                case SDLK_z:
                    chipeight.releaseKey(0xA);
                    break;
                case SDLK_x:
                    chipeight.releaseKey(0x0);
                    break;
                case SDLK_c:
                    chipeight.releaseKey(0xB);
                    break;
                case SDLK_v:
                    chipeight.releaseKey(0xF);
                    break;
                default :
                    break;
                }
            }
            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    render();
                    break;
                }
            }
        }
    }

public:
    MainLoop(int argc, char *argv[])
    {
        initSdl();
        chipeight.init();
        chipeight.load("./my.ch8");
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
        double f_deltaTime = 0;
        Uint32 currentTime = SDL_GetTicks();
        Uint32 lastTime = 0;
        Uint32 time = 0; // Используется для ограничения FPS




        running = true;
        while(running)
        {
            if (currentTime > lastTime)
                lastTime = currentTime;
            currentTime = SDL_GetTicks();
            f_deltaTime = (double)((currentTime - lastTime)/1000.0f);
            time = SDL_GetTicks();

            if(chipeight.drawFlag)
                render();
            processEvents();
            for(int i = 0; i < 10; i++)
            {
                chipeight.printDebug();
                chipeight.cycle();
            }

            double n_FPScap = 60.0;

            if(1000.0/n_FPScap > SDL_GetTicks()-time)
            {
                SDL_Delay(1000.0/n_FPScap-(double)(SDL_GetTicks()-time));
            }
        }
    }
};

#endif // MAINLOOP_H
