#pragma once

#include <SDL.h>
#include <deque>
#include <memory>

class Worker; // Forward declaration
class Entity;

struct Task {
public:
    Task(SDL_Point l, Entity* r);
    ~Task();
    SDL_Point location;
    Entity* requester;
};

class TaskSystem {
public:
    static void addTask(std::unique_ptr<Task> task);

    static void addWorker(Worker* worker);

private:
    static std::deque<std::unique_ptr<Task>> taskQueue;
    static std::deque<Worker*> workerQueue;
};