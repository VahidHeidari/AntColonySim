#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

#include <SDL_timer.h>

#include "ant.h"
#include "constants.h"
#include "food.h"
#include "graphics.h"
#include "inputs.h"
#include "pheromone.h"

extern bool is_draw_debugs;
extern bool is_draw_trails;

bool is_finished = false;
double simulation_time = 0.0;
double simulation_speed = 1.0;

AntVect ants;
FoodPileVect food_piles;
PheromoneList pheromones;
Vector2D home_pos(HOME_X, HOME_Y);



void UpdateWorld()
{
	for (double s = 0.0; s + TIME_STEP < simulation_speed; s += TIME_STEP) {
		for (unsigned i = 0; i < ants.size(); ++i)					// Update ants.
			ants[i].Update(TIME_STEP);

		PheromoneList::iterator p = pheromones.begin();				// Update pheromones.
		while (p != pheromones.end()) {
			PheromoneList::iterator next = p;
			++next;
			p->Update(TIME_STEP);
			if (p->strength < 0)
				pheromones.erase(p);
			p = next;
		}
		simulation_time += TIME_STEP;								// Update simulation time.
	}
}

void DrawGraphics()
{
	ClearBackground();												// Clear background
	DrawBorder();													// Draw world border.

	DrawHome(home_pos);												// Draw home.
	std::for_each(food_piles.begin(), food_piles.end(), [&](const auto& f) { DrawFoodPile(f); });	// Draw foods

	std::for_each(pheromones.begin(), pheromones.end(), [&](const auto& p) { DrawPheromone(p); });	// Draw pheromones.

	for (unsigned i = 0; i < ants.size(); ++i)						// Draw ants.
		DrawAnt(ants[i]);
	UpdateScreen();
}

bool SaveState(const char* out_path = "LastState.txt")
{
	std::ofstream out_file(out_path);
	if (!out_file.is_open())
		return false;

	// Simulation parameters.
	out_file << "# Simulation parameters:" << std::endl;
	out_file << "simulation_tim " << simulation_time << std::endl;
	out_file << "simulation_speed " << simulation_speed << std::endl;
	out_file << "is_draw_debugs " << std::boolalpha << is_draw_debugs << std::endl;
	out_file << "is_draw_trails " << std::boolalpha << is_draw_trails << std::endl;
	out_file << std::endl;

	// foods
	out_file << "# Foods:" << std::endl;
	for (unsigned i = 0; i < food_piles.size(); ++i)
		for (unsigned j = 0; j < food_piles[i].foods.size(); ++j)
			out_file << "food pos " << food_piles[i].foods[j].pos << std::endl;
	out_file << std::endl;

	// Ants
	out_file << "# Ants:" << std::endl;
	for (unsigned i = 0; i < ants.size(); ++i)
		out_file << "ant mass "	<< ants[i].mass
			<< " pos " << ants[i].pos
			<< " vel " << ants[i].velocity
			<< " acc " << ants[i].acceleration << std::endl;
	out_file << std::endl;

	out_file << "# Pheromones:" << std::endl;
	std::for_each(pheromones.begin(), pheromones.end(), [&](const auto& p) {
		out_file << "pheromone strength " << p.strength
			<< " pos " << p.pos << std::endl;
	});
	return true;
}

void PutFoodPile(int x, int y, int width=10, int height=10)
{
	FoodPile fp(x, y, width, height);
	food_piles.push_back(fp);
}

int main(int argc, char** argv)
{
	std::cout << "AntColonySim is about to start!" << std::endl;		// Parse command line arguments.
	for (int i = 0; i < argc; ++i) {
		std::cout << "    argv[" << i << "] : " << argv[i] << std::endl;

		if (strcmp(argv[i], "-d") == 0)
			is_draw_debugs = true;
		else if (strcmp(argv[i], "-t") == 0)
			is_draw_trails = true;
	}
	std::cout << std::endl;

	if (!InitGraphics()) {												// Initialize graphics.
		std::cout << "Could not initialize graphics!" << std::endl;
		return 1;
	}

	if (!InitInputs()) {												// Initialize inputs.
		std::cout << "Could not initialize inputs!" << std::endl;
		return 2;
	}

	// Read outputs of previous simulation for initializing new one.
	Ant ant(HOME_X, HOME_Y);
	ants.resize(NUM_ANTS, ant);

	PutFoodPile(BORDER_MARGIN * 3, BORDER_MARGIN * 3, 20, 20);
	PutFoodPile(WINDOW_WIDTH - BORDER_MARGIN * 8, BORDER_MARGIN * 3, 20, 20);
	PutFoodPile(BORDER_MARGIN * 3, WINDOW_HEIGHT - BORDER_MARGIN * 7, 20, 20);

	while (!is_finished) {												// Run simulation loop.
		HandelInputs();													// Read user inputs.
		UpdateWorld();													// Update world.
		DrawGraphics();													// Draw graphics.
		SDL_Delay(33);
	}

	SaveState();														// Save ouptpus for next run.

	CleanupGraphics();													// Cleanup everything.
	CleanupInputs();
	return 0;
}
