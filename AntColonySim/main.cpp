#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
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
extern bool is_draw_ant_numbers;

bool is_finished = false;
double simulation_time = 0.0;
double simulation_speed = 1.0;

AntVect ants;
FoodPileVect food_piles;
PheromoneGrid pheromones_grid;
Vector2D home_pos(HOME_X, HOME_Y);



void UpdateWorld()
{
	int n = 0;
	for (double s = 0.0; s < simulation_speed; s += TIME_STEP, ++n) {
		for (unsigned i = 0; i < ants.size(); ++i)						// Update ants.
			ants[i].Update(TIME_STEP);

		pheromones_grid.Update(TIME_STEP);								// Update pheromones.
		simulation_time += TIME_STEP;									// Update simulation time.
	}
}

void DrawGraphics()
{
	ClearBackground();													// Clear background
	DrawBorder();														// Draw world border.

	DrawHome(home_pos);													// Draw home.
	int num_foods = 0;
	std::for_each(food_piles.begin(), food_piles.end(), [&](const auto& f) {	// Draw foods
		DrawFoodPile(f);
		num_foods += f.foods.size();
	});

	pheromones_grid.Draw();												// Draw pheromones.

	for (unsigned i = 0; i < ants.size(); ++i) {						// Draw ants.
		DrawAnt(ants[i]);
		if (is_draw_ant_numbers) {
			SetDrawColor(255, 255, 255);
			DrawString(std::to_string(i + 1),
				static_cast<int>(ants[i].pos.x),
				static_cast<int>(ants[i].pos.y));
		}
	}

	SetDrawColor(255, 255, 255);										// Draw HUD.
	std::ostringstream oss;
	oss << "Speed     : " << simulation_speed << 'x' << std::endl;
	oss << "Time      : " << static_cast<int>(simulation_time) << std::endl;
	oss << "Num foods : " << num_foods << std::endl;
	DrawString(oss.str(), 10, 10);
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
	//std::for_each(pheromones.begin(), pheromones.end(), [&](const auto& p) {
	//	out_file << "pheromone strength " << p.strength
	//		<< " pos " << p.pos << std::endl;
	//});
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
		else if (strcmp(argv[i], "-n") == 0)
			is_draw_ant_numbers = true;
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

	// TODO: Read outputs of previous simulation for initializing new one.
	Ant ant(HOME_X, HOME_Y);
	ants.resize(NUM_ANTS, ant);

	PutFoodPile(BORDER_MARGIN * 3, BORDER_MARGIN * 3, 20, 20);
	PutFoodPile(WINDOW_WIDTH - BORDER_MARGIN * 7, BORDER_MARGIN * 3, 20, 20);
	PutFoodPile(BORDER_MARGIN * 3, WINDOW_HEIGHT - BORDER_MARGIN * 7, 20, 20);
	PutFoodPile(WINDOW_WIDTH - BORDER_MARGIN * 7, WINDOW_HEIGHT - BORDER_MARGIN * 7, 20, 20);

	pheromones_grid.Init(WINDOW_HEIGHT / CELL_HEIGHT_PX + 1, WINDOW_WIDTH / CELL_WIDTH_PX + 1);

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
