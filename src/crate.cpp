#include "crate.h"

#include "globals.h"
#include "tilemap.h"
#include "tasks.h"

Crate::Crate(SDL_Point position) : Entity(position) {
    src = {16, 8, 8, 8};
    dest = {0, 0, 40, 40};

    // Choose a slight variation +- 10 degrees and flip
    angle = 10 - (rand() % 20);
    flip = SDL_RendererFlip(rand() % 2);

    // Add a task to the task system
    TaskSystem::addTask(std::make_unique<Task>(position, this));
}

void Crate::update(bool* shouldDestroy) {
    // Check if the task is complete
    if (taskComplete) {
        // Request to be destroyed
        *shouldDestroy = true;
    }
}