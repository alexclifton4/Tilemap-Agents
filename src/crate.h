#pragma once

#include <SDL.h>
#include "entity.h"

class Crate: public Entity {
public:
    Crate(SDL_Point position);

    void update(bool* shouldDestroy);
};