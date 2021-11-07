#include "player.h"

#include "globals.h"
#include "tilemap.h"
#include "crate.h"
#include "tasks.h"

Player::Player() {
    position = Tilemap::tile2world({1, 1});
}

void Player::update() {
    SDL_FPoint movement = {0, 0};

    // See if arrow keys are pressed
    // Not using elseif - therefore if e.g. up and down pressed, they cancel
    if (g_keyState[SDL_SCANCODE_UP] || g_keyState[SDL_SCANCODE_W]) {
        movement.y -= 1;
    }
    if (g_keyState[SDL_SCANCODE_DOWN] || g_keyState[SDL_SCANCODE_S]) {
        movement.y += 1;
    }
    if (g_keyState[SDL_SCANCODE_LEFT] || g_keyState[SDL_SCANCODE_A]) {
        movement.x -= 1;
    }
    if (g_keyState[SDL_SCANCODE_RIGHT] || g_keyState[SDL_SCANCODE_D]) {
        movement.x += 1;
    }

    // Normalise the movement vector - if moving diagonally, the speed will be more than 1
    float mag = sqrt(movement.x * movement.x + movement.y * movement.y);
    if (mag != 0 && mag != 1) {
        movement.x = movement.x / mag;
        movement.y = movement.y / mag;
    }

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
    if (g_keyDown[SDL_SCANCODE_SPACE]) {
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