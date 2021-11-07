#include "entity.h"

#include "tilemap.h"
#include "globals.h"

Entity::Entity(SDL_Point pos) {
    position = Tilemap::tile2world(pos);
}

void Entity::render(SDL_Texture* tiles) {
    // Offset by camera position
    dest.x = position.x - 20 - g_camera.x;
    dest.y = position.y - 20 - g_camera.y;

    // See if the entity is in an error state
    if (errorState) {
        // Pulse the size by up to 10%
        SDL_Rect errorDest = dest;
        errorDest.w = dest.w + (sin(SDL_GetTicks() / 100) * dest.w / 10);
        errorDest.h = dest.h + (sin(SDL_GetTicks() / 100) * dest.h / 10);

        SDL_RenderCopyEx(g_renderer, tiles, &src, &errorDest, angle, NULL, flip);        
    } else {
        // Render normally
        SDL_RenderCopyEx(g_renderer, tiles, &src, &dest, angle, NULL, flip);
    }
}