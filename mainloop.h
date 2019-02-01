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

    void initSdl();
    void render();
    void processEvents();

public:
    MainLoop(int argc, char *argv[]);
    ~MainLoop();

    void run();
};

#endif // MAINLOOP_H
