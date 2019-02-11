#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>

class Audio
{
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer = nullptr;
    SDL_AudioDeviceID deviceId;

public:
    Audio();
    ~Audio();

    void init();

    void play();
};

#endif // AUDIO_H
