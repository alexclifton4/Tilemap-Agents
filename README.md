# Tilemap
## Task system
Many management simulations games have a mechanic where workers carry out tasks, for example the contruction workers in Prison Architect, and I wanted to implement something like this.

There is a task system which workers can register with, and that tasks can be added to. When a task is created, it will immediately be assigned to a worker if one is available, otherwise it will be added to a queue. Likewise, workers will be given waiting tasks when they register, or added to a queue if there are no current tasks.

Using queues for both the workers and tasks is a fairly basic approach and I would like to improve this to work out which task and worker are closest to each other.

The workers use Breadth-first search to find their target. Something like A* would be more efficient for this, but it doesn't affect performance in this demo.

## Implementation
The demo is made in C++ using SDL for graphics. This is the first time I've used Emscripten to compile C++ for the web, and I also managed to port it to Android.

The demo can be found here: [https://alexclifton.co.uk/games](https://alexclifton.co.uk/games).
