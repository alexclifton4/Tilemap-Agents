#pragma once

#include <SDL.h>
#include <queue>
#include <vector>
#include "tilemap.h"

namespace Pathfinding {
    std::deque<SDL_Point> findPath(SDL_Point start, SDL_Point target);
    void findNeighbours(SDL_Point point, std::vector<std::vector<Tile>>* map, std::queue<SDL_Point>* queue);
}