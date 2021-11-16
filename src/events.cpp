#include "events.h"
#include "globals.h"

SDL_FPoint Events::playerMovement;
bool Events::placeBox;
bool Events::fingerDown = false;
SDL_FPoint Events::fingerStart;
SDL_FPoint Events::fingerDelta;

// Update inputs every frame
void Events::updateInput(bool* playing) {
    // Reset state
	placeBox = false;
	playerMovement = {0, 0};

	// Check events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
			*playing = false;
			break;

			case SDL_KEYDOWN:
			// Ignore repeated keys
			if (event.key.repeat == 0) {
				// A box should be placed when space is pressed
                if (event.key.keysym.sym == SDLK_SPACE) {
					placeBox = true;
                }
			}
			break;

			case SDL_FINGERDOWN:
			fingerDown = true;
			fingerStart = {event.tfinger.x, event.tfinger.y};
			break;

			case SDL_FINGERUP:
			fingerDown = false;
			fingerDelta = {0, 0};
			break;

			case SDL_FINGERMOTION:
			// See how far the finger has moved from the start
			fingerDelta = {event.tfinger.x - fingerStart.x, event.tfinger.y - fingerStart.y};
			break;

			case SDL_MOUSEBUTTONDOWN:
			// Check for double tap
			if (event.button.which == SDL_TOUCH_MOUSEID && event.button.clicks == 2) {
				// On touchscreens, place a box on double tap
				placeBox = true;
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

	// Update player movement from keyboard
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	// Not using elseif - therefore if e.g. up and down pressed, they cancel
	if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) {
        playerMovement.y -= 1;
    }
    if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) {
        playerMovement.y += 1;
    }
    if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) {
        playerMovement.x -= 1;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) {
        playerMovement.x += 1;
    }

	// Update player movement from touchscreen
	if (fingerDelta.x > 0.1) {
		playerMovement.x += 1;
	}
	if (fingerDelta.x < -0.1) {
		playerMovement.x -= 1;
	}
	if (fingerDelta.y > 0.1) {
		playerMovement.y += 1;
	}
	if (fingerDelta.y < -0.1) {
		playerMovement.y -= 1;
	}

	// Normalise the movement vector - if moving diagonally, the speed will be more than 1
    float mag = sqrt(playerMovement.x * playerMovement.x + playerMovement.y * playerMovement.y);
    if (mag != 0 && mag != 1) {
        playerMovement.x = playerMovement.x / mag;
        playerMovement.y = playerMovement.y / mag;
    }
}