#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory>

#include "tilemap.h"
#include "player.h"
#include "crate.h"
#include "worker.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

// Global variables
int g_window_width = 640;
int g_window_height = 480;
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;
SDL_FPoint g_camera = {0, 0};
std::unique_ptr<Tilemap> g_map;
std::vector<std::unique_ptr<Entity>> g_entities;
const Uint8* g_keyState;
Uint8 g_keyDown[512];
double g_deltaTime = 0;

SDL_Texture* tiles = NULL;
bool playing = true;
Uint64 now;
Uint64 then;

std::unique_ptr<Player> player;

// Setup SDL
bool initSDL() {
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("Unable to initialise SDL: %s\n", SDL_GetError());
		return 0;
	}

	// Create the window and renderer
	g_window = SDL_CreateWindow("Tilemap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_window_width, g_window_height, SDL_WINDOW_RESIZABLE);
	if (g_window == NULL) {
		printf("Unable to create window: %s\n", SDL_GetError());
		return 0;
	}
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_renderer == NULL) {
		printf("Unable to create renderer: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

// Clean up when finished
void quit() {
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	SDL_Quit();
}

// Load a texture from a file
SDL_Texture* loadTexture(const char* path) {
	// Create a surface
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL) {
		printf("Unable to create surface: %s\n", SDL_GetError());
		return NULL;
	}

	// Create a texture from the surface, then free the surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	if (texture == NULL) {
		printf("Unable to load texture: %s\n", SDL_GetError());
		return NULL;
	}
	SDL_FreeSurface(surface);

	return texture;
}

// Main game loop
void mainLoop() {
	// Calculate delta
	then = now;
	now = SDL_GetPerformanceCounter();
	g_deltaTime = (double)((now - then) / (double)SDL_GetPerformanceFrequency());

	// Reset keys down
	for (int i = 0; i < 512; i++) {
		g_keyDown[i] = 0;
	}

	// Check events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
			playing = false;
			break;

			case SDL_KEYDOWN:
			// Ignore repeated keys
			if (event.key.repeat == 0) {
				// Record which key was pressed
				g_keyDown[event.key.keysym.scancode] = 1;
			}
			break;

			case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				g_window_width = event.window.data1;
				g_window_height = event.window.data2;
			}
			break;
		}
	}

	// Update key state
	g_keyState = SDL_GetKeyboardState(NULL);

	// Update the player
	player->update();

	// Update entities and workers
	auto entity = g_entities.begin();
	while (entity != g_entities.end()) {
		bool shouldDestroy = false;
		entity->get()->update(&shouldDestroy);

		// Destroy the entity if it requested it
		if (shouldDestroy) {
			entity = g_entities.erase(entity);
		} else {
			++entity;
		}
	}

	// Move the camera to the player's location
	// Offset by half the screen size
	g_camera.x = player->position.x - g_window_width / 2;
	g_camera.y = player->position.y - g_window_height / 2;

	// Clear the screen before rendering
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(g_renderer);

	// Render everything
	g_map->render(tiles);
	for (auto it = g_entities.rbegin(); it != g_entities.rend(); ++it) {
		it->get()->render(tiles);
	}
	player->render(tiles);

	// Present the rendererd frame
	SDL_RenderPresent(g_renderer);
}

// Program entry point
int main(int argc, char* args[]) {
	// Seed the PRNG
	srand(time(NULL));

	// Initialise SDL
	bool success = initSDL();

	if (success) {
		// Load the tiles
		tiles = loadTexture("tiles.png");

		if (tiles == NULL) {
			quit();
			return 1;
		}

		// Initialise the map
		g_map = std::make_unique<Tilemap>();

		// Create the player
		player = std::make_unique<Player>();

		// Create some workers
		g_entities.push_back(std::make_unique<Worker>((SDL_Point){3, 3}));
		g_entities.push_back(std::make_unique<Worker>((SDL_Point){4, 4}));
		g_entities.push_back(std::make_unique<Worker>((SDL_Point){5, 5}));

		// Run the main loop
		now = SDL_GetPerformanceCounter();
		then = 0;
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(mainLoop, 0, 1);
#else
		while (playing) {
			mainLoop();
		}
#endif

		// Clean up textures
		SDL_DestroyTexture(tiles);

		// Clean up when finished
		quit();
	}

	return 0;
}