#pragma once

#include <SDL.h>

namespace Events {
    void updateInput(bool* playing);

    extern SDL_FPoint playerMovement;
    extern bool placeBox;

    extern bool fingerDown;
    extern SDL_FPoint fingerStart;
    extern SDL_FPoint fingerDelta;
}