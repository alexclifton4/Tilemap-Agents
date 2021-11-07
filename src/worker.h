#pragma once

#include <SDL.h>
#include "entity.h"
#include "tasks.h"

class Worker: public Entity {
public:
    Worker(SDL_Point position);

    void update(bool* shouldDestroy);
    void assignTask(std::unique_ptr<Task> task);

private:
    const int speed = 100;
    std::unique_ptr<Task> task = nullptr;
    std::deque<SDL_Point> path;
};