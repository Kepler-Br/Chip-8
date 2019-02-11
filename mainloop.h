#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include <stdexcept>
#include <tuple>

#include "chipeight.h"
#include "audio.h"

class MainLoop
{
private:
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;

    ChipEight chipeight;
    Audio audio;
    int windowHeight = 480;
    int windowWidth = 640;
    // Hz for Chip-8. How many cycles per second would be calculated.
    int cyclesPerSecond = 600;
    // How many cycles would app perform per second.
    //Basically - FPS
    int loopsPerSecond = 30;
    std::tuple<uint8_t, uint8_t, uint8_t> backgroundColor;
    std::tuple<uint8_t, uint8_t, uint8_t> pixelColor;

    void initSdl();
    void render();
    void processEvents();

    void readConfig();

public:
    MainLoop(int argc, char *argv[]);
    ~MainLoop();

    void run();
};

#endif // MAINLOOP_H
