#include "worker.h"

#include "globals.h"
#include "tilemap.h"
#include "tasks.h"
#include "pathfinding.h"

Worker::Worker(SDL_Point position) : Entity(position){
    home = position;
    src = {8, 8, 8, 8};
    dest = {0, 0, 40, 40};
    state = IDLE;

    // Register with task system
    TaskSystem::addWorker(this);
}

void Worker::update(bool* shouldDestroy) {
    // Update depending on state
    switch (state)
    {
    case IDLE:
        // Don't do anything in this state
        // See if we have been assigned a task
        if (task) {
            // Go to the task start point
            path = Pathfinding::findPath(Tilemap::world2tile(position), task->start);
            state = TO_START;
        } else {
            // Plan a route home
            if (path.empty()) {
                path = Pathfinding::findPath(Tilemap::world2tile(position), home);
            }
            // Go home
            followPath();
        }
        break;

    case TO_START:
        // Follow the path to the start point
        followPath();
        // See if we have reached the start point
        if (path.empty()) {
            // Pick up the entity
            carrying = true;
            task->requester->carried = true;
            // Go to the task end point
            path = Pathfinding::findPath(Tilemap::world2tile(position), task->end);
            state = TO_END;
        }
        break;

    case TO_END:
        // Follow the path to the end point
        followPath();
        // See if we have reached the end point
        if (path.empty()) {
            // Start working on it
            state = WORKING;
        }
        break;

    case WORKING:
        if (task->timeRemaining > 0) {
            // Wait by the task for it to complete
            task->timeRemaining -= g_deltaTime;
        } else {
            // The task is complete
            // Stop carrying the entity
            carrying = false;
            task->requester->carried = false;
            // Destroy the task and re-register with the task system
            task = nullptr;
            TaskSystem::addWorker(this); // This might assign a new task immediately

            // Move to next state, depending on if a task was assigned
            if (task) {
                path = Pathfinding::findPath(Tilemap::world2tile(position), task->start);
                state = TO_START;
            } else {
                state = IDLE;
            }
        }
        break;
    }
}

void Worker::followPath() {
    // Find the next tile in the path
    SDL_FPoint nextPoint = Tilemap::tile2world(path.front());

    // Find vector towards next point
    SDL_FPoint vector = {nextPoint.x - position.x, nextPoint.y - position.y};
    float mag = sqrt(vector.x * vector.x + vector.y * vector.y);

    // See if the next point has been reached
    if (mag == 0) {
        // Pop the point from the path
        path.pop_front();
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

void Worker::render(SDL_Texture* tiles) {
    // Render entity as normal
    Entity::render(tiles);

    // See if we are carrying something
    if (carrying) {
        // Render the entity as well
        SDL_Rect dest;
        dest.x = position.x - 20 - g_camera.x; // Offset by camera
        dest.y = position.y - 20 - g_camera.y; // Offset by camera
        dest.w = 30;
        dest.h = 30;

        SDL_RenderCopy(g_renderer, tiles, &task->requester->src, &dest);
    }

    // See if we are at a task site
    if (task && path.empty()) {
        // Render progress bar above the worker
        SDL_Rect bar;
        bar.x = position.x - 20 - g_camera.x; // Offset by camera
        bar.y = position.y - 20 - g_camera.y - 15; // Offset by camera, and above the worker
        bar.w = 40;
        bar.h = 10;
        // Draw the outer rectangle
        SDL_SetRenderDrawColor(g_renderer, 220, 220, 220, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(g_renderer, &bar);

        // Fill the rectangle
        bar.w = 40 * (task->duration - task->timeRemaining) / task->duration;
        SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(g_renderer, &bar);
    }
}

void Worker::assignTask(std::unique_ptr<Task> t) {
    task = std::move(t);
}