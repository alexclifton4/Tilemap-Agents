#include "tasks.h"

#include <SDL.h>
#include "worker.h"

std::deque<std::unique_ptr<Task>> TaskSystem::taskQueue;
std::deque<Worker*> TaskSystem::workerQueue;

Task::Task(SDL_Point s, SDL_Point e, double d, Entity* r) {
    start = s;
    end = e;
    timeRemaining = duration = d;
    requester = r;
}

Task::~Task() {
    // Mark as done at the requester
    requester->taskComplete = true;
}

void TaskSystem::addTask(std::unique_ptr<Task> task) {
    // See if there are any workers available to take the task
    if (workerQueue.empty()) {
        // No available workers, add to task queue
        taskQueue.push_back(std::move(task));
    } else {
        // Get a worker from the queue
        Worker* worker = workerQueue.front();
        workerQueue.pop_front();

        // Assign the task to the worker
        worker->assignTask(std::move(task));
    }
}

void TaskSystem::addWorker(Worker* worker) {
    // See if there are any tasks in the queue
    if (taskQueue.empty()) {
        // No tasks to take, add the worker to the queue
        workerQueue.push_back(worker);
    } else {
        // Get a task from the queue and assign to worker
        worker->assignTask(std::move(taskQueue.front()));
        taskQueue.pop_front();
    }
}