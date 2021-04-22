#include "inputs.h"

#include <SDL.h>

#include "constants.h"

extern bool is_finished;
extern double simulation_speed;
extern bool is_draw_debugs_circle;
extern bool is_draw_debugs_bounding_box;
extern bool is_draw_trails;

bool is_draw_ant_numbers = false;

static const Uint32 INIT_FLAGS =
	SDL_INIT_EVENTS |
	SDL_INIT_GAMECONTROLLER |
	SDL_INIT_JOYSTICK;



bool InitInputs()
{
	if (SDL_InitSubSystem(INIT_FLAGS) < 0)
		return false;
	return true;
}

void CleanupInputs()
{
	SDL_QuitSubSystem(INIT_FLAGS);
}

void HandelInputs()
{
	SDL_Event sdl_event;
	while (SDL_PollEvent(&sdl_event)) {
		switch (sdl_event.type) {
		case SDL_WINDOWEVENT:
			if (sdl_event.window.event != SDL_WINDOWEVENT_CLOSE)
				break;

			is_finished = true;
			break;

		case SDL_QUIT:
			is_finished = true;
			break;

		case SDL_KEYDOWN:
			switch (sdl_event.key.keysym.sym) {
			case SDLK_a:
				if (simulation_speed)
					simulation_speed = 0.0;
				else
					simulation_speed = 1.0;
				break;

			case SDLK_b:
				is_draw_debugs_bounding_box ^= true;
				break;

			case SDLK_d:
				is_draw_debugs_circle ^= 1;
				break;

			case SDLK_ESCAPE:
				is_finished = true;
				break;

			case SDLK_n:
				is_draw_ant_numbers ^= true;
				break;

			case SDLK_s:
				if (simulation_speed < MAX_SIMULATION_SPEED)
					simulation_speed *= 2.0;
				else
					simulation_speed = 1.0;
				break;

			case SDLK_t:
				is_draw_trails ^= 1;
				break;
			}
			break;
		}
	}
}
