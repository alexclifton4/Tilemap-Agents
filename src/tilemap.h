#pragma once

#include <SDL.h>
#include <vector>

struct Tile {
public:
    SDL_Rect texture;
    bool walkable;

    // Used during pathfinding
    bool visited;
    SDL_Point parent;
};

class Tilemap {
public:
    Tilemap();
    ~Tilemap() {};

    std::vector<std::vector<Tile>> map;
    SDL_Point mapSize;

    void render(SDL_Texture* tiles);

    static SDL_Point world2tile(SDL_FPoint world);
    static SDL_FPoint tile2world(SDL_Point tile);

private:
    std::vector<Tile> tiles;

    static const int tileSize = 50;

    void loadMap(const char* file);
};