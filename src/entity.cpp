#include "entity.h"

#include "tilemap.h"
#include "globals.h"

Entity::Entity(SDL_Point pos) {
    position = Tilemap::tile2world(pos);
}

void Entity::render(SDL_Texture* tiles) {
    // Only render if it's not being carried
    if (!carried) {
        // Offset by camera position
        dest.x = position.x - 20 - g_camera.x;
        dest.y = position.y - 20 - g_camera.y;

        SDL_RenderCopyEx(g_renderer, tiles, &src, &dest, angle, NULL, flip);
    }
}