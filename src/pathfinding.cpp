#include "pathfinding.h"

#include "globals.h"

using Map = std::vector<std::vector<Tile>>*;

std::deque<SDL_Point> Pathfinding::findPath(SDL_Point start, SDL_Point target) {
    std::deque<SDL_Point> path;

    // Return a single point if start = target
    if (start.x == target.x && start.y == target.y) {
        path.push_front({target.x, target.y});

        return path;
    }

    std::queue<SDL_Point> queue;
    SDL_Point nextPoint;
    bool pathPossible = true;
    Map map = &g_map->map; // Local reference to map

    // Initialise the map by setting all tiles to not visited
    for (int x = 0; x < g_map->mapSize.x; x++) {
        for (int y = 0; y < g_map->mapSize.y; y++) {
            g_map->map[y][x].visited = false;
        }
    }

    // Begin at the start point
    queue.push(start);

    // Loop until the target is found
    while (!(nextPoint.x == target.x && nextPoint.y == target.y)) {
        if (queue.empty()) {
            SDL_Log("No path possible from %d:%d to %d:%d", start.x, start.y, target.x, target.y);
            pathPossible = false;
            break;
        }

        // Pop a point off the queue and mark it as visited
        nextPoint = queue.front();
        queue.pop();

        // Find the tile's neighbours
        findNeighbours(nextPoint, map, &queue);
    }

    if (pathPossible) {
        // Backtrack from the target, adding parent points to path
        while (!(nextPoint.x == start.x && nextPoint.y == start.y)) {
            path.push_front(nextPoint);
            nextPoint = (*map)[nextPoint.y][nextPoint.x].parent;
        }
    }

    return path;
}

void Pathfinding::findNeighbours(SDL_Point point, Map map, std::queue<SDL_Point>* queue) {
    // Define search directions
    SDL_Point directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int i = 0; i < 8; i++) {
        // Work out neighbour point
        SDL_Point neighbour = {point.x + directions[i].x, point.y + directions[i].y};
        Tile* tile = &(*map)[neighbour.y][neighbour.x];

        // Ignore if it's outside the map boundary
        if (neighbour.x < 0 || neighbour.y < 0) {
            continue;
        }
        if (neighbour.x >= g_map->mapSize.x || neighbour.y >= g_map->mapSize.y) {
            continue;
        }

        // Check if the tile has not been visited and is walkable
        if (!tile->visited && tile->walkable) {
            // Mark the point as visited
            tile->visited = true;
            // Mark its parent
            tile->parent = point;
            // Push the tiles to the pathfinding queue
            queue->push(neighbour);
        }
    }
}