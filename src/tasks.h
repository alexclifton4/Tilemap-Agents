#pragma once

#include <SDL.h>
#include <deque>
#include <memory>

class Worker; // Forward declaration
class Entity;

struct Task {
public:
    Task(SDL_Point start, SDL_Point end, double duration, Entity* requester);
    ~Task();
    SDL_Point start;
    SDL_Point end;
    double duration;
    double timeRemaining;
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