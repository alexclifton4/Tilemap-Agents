#include "tilemap.h"
#include <stdlib.h>
#include <cctype>

#include "globals.h"

Tilemap::Tilemap() {
    // Define some tile types
    tiles.push_back({{0, 0, 8, 8}, true}); // Grass
    tiles.push_back({{8, 0, 8, 8}, true}); // Floor
    tiles.push_back({{16, 0, 8, 8}, false}); // Wall

    // Load a map from file
    loadMap("map.txt");
}

// Draw the tilemap
void Tilemap::render(SDL_Texture* tiles) {
    SDL_Rect destination;
    destination.w = destination.h = tileSize;

    // Loop through the map
    for (int x = 0; x < mapSize.x; x++) {
        for (int y = 0; y < mapSize.y; y++) {
            // Calculate destination rect
            // Offset by camera position
            destination.x = (tileSize * x) - g_camera.x;
            destination.y = (tileSize * y) - g_camera.y;

            // Render the tile
            SDL_RenderCopy(g_renderer, tiles, &map[y][x].texture, &destination);
        }
    }
}

// Convert between coordinate systems
SDL_Point Tilemap::world2tile(SDL_FPoint world){
    SDL_Point tile;
    tile.x = world.x / tileSize;
    tile.y = world.y / tileSize;

    return tile;
}

// Convert between coordinate systems
SDL_FPoint Tilemap::tile2world(SDL_Point tile) {
    SDL_FPoint world;
    world.x = tile.x * tileSize + (tileSize / 2);
    world.y = tile.y * tileSize + (tileSize / 2);

    return world;
}

// Loads a map from a file
void Tilemap::loadMap(const char* fileName) {
    // Open a map file
    SDL_RWops* file = SDL_RWFromFile(fileName, "r");
    Sint64 fileSize = SDL_RWsize(file);

    // Loop through the file
    std::vector<Tile> row;
    for (int i = 0; i < fileSize; i++) {
        // Read a character
        char c;
        file->read(file, &c, sizeof(char), 1);

        // Process the character
        if (isdigit(c)) {
            // Represents a tile
            int type = c - '0'; // Converts ASCII to int
            row.push_back(tiles[type]);
        } else if (c == '\n') {
            // End of a row
            // Append to map and reset
            map.push_back(row);
            row.clear();
        }
        // Ignore all other characters
    }

    mapSize.y = map.size();
    mapSize.x = map[0].size();
}