#pragma once

#include <SDL.h>
#include "entity.h"
#include "tasks.h"

class Worker: public Entity {
public:
    Worker(SDL_Point position);

    void update(bool* shouldDestroy);
    void render(SDL_Texture* tiles);
    void assignTask(std::unique_ptr<Task> task);

private:
    enum State {
        IDLE,
        TO_START,
        TO_END,
        WORKING
    };

    const int speed = 100;
    SDL_Point home;
    State state;
    std::unique_ptr<Task> task = nullptr;
    std::deque<SDL_Point> path;
    bool carrying = false;

    void followPath();
};