#include "worker.h"

#include "globals.h"
#include "tilemap.h"
#include "tasks.h"
#include "pathfinding.h"

Worker::Worker(SDL_Point position) : Entity(position){
    src = {8, 8, 8, 8};
    dest = {0, 0, 40, 40};

    // Register with task system
    TaskSystem::addWorker(this);
}

void Worker::update(bool* shouldDestroy) {
    // If a task is assigned, execute it
    if (task) {
        // Find the next tile in the path
        SDL_FPoint nextPoint = Tilemap::tile2world(path.front());

        // Find vector towards next point
        SDL_FPoint vector = {nextPoint.x - position.x, nextPoint.y - position.y};
        float mag = sqrt(vector.x * vector.x + vector.y * vector.y);
        
        // See if the next point has been reached
        if (mag == 0) {
            // Pop the point from the path
            path.pop_front();

            // If the path is now empty, we have reached the task site
            if (path.empty()) {

                // Destroy the task and re-register with task system
                task = nullptr;
                TaskSystem::addWorker(this);
            }
        } else {
            // Move towards the next point
            // Normalise vector to get direction vector
            SDL_FPoint movement;
            movement.x = vector.x / mag;
            movement.y = vector.y / mag;

            // Multiply by speed and delta time
            movement.x = movement.x * speed * (float)g_deltaTime;
            movement.y = movement.y * speed * (float)g_deltaTime;

            // Clamp to distance
            if (abs(movement.x) > abs(vector.x)) movement.x = vector.x;
            if (abs(movement.y) > abs(vector.y)) movement.y = vector.y;

            // Move the worker
            position.x += movement.x;
            position.y += movement.y;
        }
    }
}

void Worker::assignTask(std::unique_ptr<Task> t) {
    task = std::move(t);

    // Find a route to the task
    path = Pathfinding::findPath(Tilemap::world2tile(position), task->location);

    // Check if a path was returned
    if (path.empty()) {
        // A path was not found, enter an error state
        SDL_Log("Worker could not find route to task");
        errorState = true;

        // Destroy the task
        // This isn't ideal, an actual game would need a better implementation
        task = nullptr;
    }
}