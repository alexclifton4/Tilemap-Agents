#pragma once

#include <SDL.h>
#include <memory>

class Entity {
public:
    Entity(SDL_Point position);
    ~Entity() {}

    virtual void update(bool* shouldDestroy) = 0;
    virtual void render(SDL_Texture* tiles);

    SDL_Rect src = {0, 0, 0, 0};
    bool carried = false;
    bool taskComplete = false;

protected:
    SDL_FPoint position;
    double angle = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect dest = {0, 0, 0, 0};
};