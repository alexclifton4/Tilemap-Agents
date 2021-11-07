#pragma once

#include <SDL.h>

class Player {
public:
    Player();
    ~Player() {};

    void update();
    void render(SDL_Texture* tiles);

    SDL_FPoint position;

private:
    SDL_Rect src = {0, 8, 8 ,8};
    SDL_Rect dest = {0, 0, 40, 40};
    const int speed = 200;
};