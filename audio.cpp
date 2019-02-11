#include "audio.h"

#include <iostream>

Audio::Audio()
{
}

Audio::~Audio()
{
    if(wavBuffer)
    {
        SDL_FreeWAV(wavBuffer);
        SDL_CloseAudioDevice(deviceId);
    }
}

void Audio::init()
{
    if(wavBuffer)
        return;
    if(SDL_LoadWAV("beep.wav", &wavSpec, &wavBuffer, &wavLength) == nullptr)
    {
        std::cout << "Cannot load beep.wav.\n";
        std::cout << "SDL error: " << SDL_GetError() << "\n";
        return;
    }
    deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
}

void Audio::play()
{
    if(!wavBuffer)
        return;
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);

}
