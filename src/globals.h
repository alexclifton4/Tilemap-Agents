#pragma once

#include <SDL.h>
#include <memory>
#include "tilemap.h"
#include "entity.h"

// Global SDL variables
extern int g_window_width;
extern int g_window_height;
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern SDL_FPoint g_camera;
extern std::unique_ptr<Tilemap> g_map;
extern std::vector<std::unique_ptr<Entity>> g_entities;
extern double g_deltaTime;