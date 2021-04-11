#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/// Graphics constants.
constexpr int WINDOW_X = 100;
constexpr int WINDOW_Y = 100;

//constexpr int WINDOW_WIDTH = 640;
//constexpr int WINDOW_HEIGHT = 480;
constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 700;

constexpr bool IS_FULLSCREEN = false;
constexpr char* WINDOW_TITLE = "AntColonySim";

constexpr int PHEROMONE_WIDTH = 5;
constexpr int PHEROMONE_HEIGHT = 5;
constexpr int FOOD_WIDTH = 5;
constexpr int FOOD_HEIGHT = 5;

/// Simulation constants.
constexpr int NUM_ANTS = 50;
constexpr double MAX_SIMULATION_SPEED = 16.0;
constexpr double TIME_STEP = 0.001;
constexpr double SENSOR_RADIUS = 20;
constexpr double SENSOR_RADIUS_2 = SENSOR_RADIUS * SENSOR_RADIUS;
constexpr double FORCE_UPDATE_TIME = 2.5;
constexpr double TRAIL_UPDATE_TIME = 5.0;
constexpr double PHEROMONE_STRENGTH = 70.0;
constexpr double PHEROMONE_DECREASE_RATE = 1;
constexpr double PHEROMONE_UPDATE_TIME = FORCE_UPDATE_TIME / 2.0;

/// World constants
constexpr int BORDER_MARGIN = 25;
constexpr double DEFAULT_ANT_MASS = 0.001;
constexpr double MAX_SPEED = 5.0;
constexpr double MAX_SPEED_2 = MAX_SPEED * MAX_SPEED;

constexpr double HOME_X = WINDOW_WIDTH / 2;
constexpr double HOME_Y = WINDOW_HEIGHT / 2;
//constexpr double HOME_X = WINDOW_WIDTH - 3 * BORDER_MARGIN;
//constexpr double HOME_Y = WINDOW_HEIGHT - 3 * BORDER_MARGIN;

#endif
