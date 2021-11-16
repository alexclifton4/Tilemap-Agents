#include "player.h"

#include "globals.h"
#include "tilemap.h"
#include "crate.h"
#include "tasks.h"
#include "events.h"

Player::Player() {
    position = Tilemap::tile2world({1, 1});
}

void Player::update() {
    // Calculate player movement
    SDL_FPoint movement = Events::playerMovement;

    // Multiply by deltatime and speed
    movement.x *= g_deltaTime * speed;
    movement.y *= g_deltaTime * speed;

    // Calculate the new position
    SDL_FPoint targetPosition;
    targetPosition.x = position.x + movement.x;
    targetPosition.y = position.y + movement.y;

    // See if the new position is walkable
    SDL_Point targetTile = Tilemap::world2tile(targetPosition);
    if (g_map->map[targetTile.y][targetTile.x].walkable) {
        // Move the player to the target position
        position = targetPosition;
    }

    // Check if a box should be placed
    if (Events::placeBox) {
        SDL_Point cratePosition = Tilemap::world2tile(position);
        g_entities.push_back(std::make_unique<Crate>(cratePosition));
    }
}

void Player::render(SDL_Texture* tiles) {
    // Offset by camera position
    dest.x = position.x - 20 - g_camera.x;
    dest.y = position.y - 20 - g_camera.y;

    SDL_RenderCopy(g_renderer, tiles, &src, &dest);
}