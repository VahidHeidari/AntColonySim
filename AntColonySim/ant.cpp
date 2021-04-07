#include "ant.h"

#include <algorithm>
#include <random>

#include "constants.h"
#include "food.h"
#include "pheromone.h"

extern FoodPileVect food_piles;
extern PheromoneList pheromones;
extern Vector2D home_pos;

std::random_device rd;
std::uniform_real_distribution<double> ud;



void Ant::Init()
{
	state = AntState::AS_WANDER;
	force_update_time = FORCE_UPDATE_TIME;
	trail_update_time = 0.0;
	pheromone_update_time = 0.0;
	mass = DEFAULT_ANT_MASS;
	pos.Init();
	velocity.Init();
	acceleration.Init();
}
#include <iostream>
void Ant::Update(double dt)
{
	if (!IsState(AntState::AS_CARRY_FOOD)) {
		for (unsigned i = 0; i < food_piles.size(); ++i) {				// Sense nearby food.
			if (food_piles[i].CheckCollision(pos)) {
				ChangeState(AntState::AS_CARRY_FOOD);					// Change state.
				if (food_piles[i].IsEmpty())
					food_piles.erase(food_piles.begin() + i);			// Remove food pile from world.
				break;
			}
		}
	} else {															// Carry food to home.
		if (pos.Distance2(home_pos) < SENSOR_RADIUS_2) {
			Init();
			pos = home_pos;
		}
	}

	Vector2D pherom_f;													// Update pheromone state.
	if (pheromone_update_time > PHEROMONE_UPDATE_TIME) {
		pheromone_update_time = 0;

		Vector2D norm = Vector2D::Normalize(velocity);
		for (PheromoneList::const_iterator p = pheromones.begin(); p != pheromones.end(); ++p) {
			if (pos.Distance2(p->pos) < SENSOR_RADIUS_2) {				// Sense nearby pheromone
				Vector2D p_dir = p->pos - pos;							// Check pheromone direction.
				if (norm.Dot(p_dir) < 0)
					continue;

				Vector2D pherom_dir = (p->pos - pos).Normalize();
				pherom_f = pherom_f + pherom_dir.Multiply(IsState(AntState::AS_CARRY_FOOD) ? 0.2 : 5.2);
			}
		}
		if (IsState(AntState::AS_CARRY_FOOD)) {							// Drop pheromones.
			Pheromone pherom(Vector2D::Normalize(velocity) + pos);
			pheromones.push_back(pherom);
		}

		pherom_f.Normalize();
		pherom_f.Multiply(0.3);
		acceleration = acceleration + (1.0 / mass) * pherom_f;
	}
	pheromone_update_time += dt;

	// Update trail.
	if (trail_update_time > TRAIL_UPDATE_TIME) {
		trail_update_time = 0;
		trail.PushBack(pos);
	}
	trail_update_time += dt;

	// Update forces.
	if (force_update_time >= FORCE_UPDATE_TIME) {						// Add wander force.
		force_update_time = 0;

		Vector2D f = Vector2D(ud(rd) - 0.5, ud(rd) - 0.5).Normalize().Multiply(1);
		Vector2D dir = velocity.Normalize().Multiply(2);
		f = dir + f;

		if (pos.x < BORDER_MARGIN)
			f.x = 1;
		if (pos.x > WINDOW_WIDTH - BORDER_MARGIN)
			f.x = -1;
		if (pos.y < BORDER_MARGIN)
			f.y = 1;
		if (pos.y > WINDOW_HEIGHT - BORDER_MARGIN)
			f.y = -1;
		f.Normalize();
		f.Multiply(0.5);

		if (IsState(AntState::AS_CARRY_FOOD)) {							// Add a little force to home.
			Vector2D home_dir = (home_pos - pos).Normalize();
			Vector2D home_f = home_dir.Multiply(0.2);
			f = f + home_f;
		}

		acceleration = (1.0 / mass) * f;
	}
	force_update_time += dt;

	// Update position.
	velocity = velocity + (dt * acceleration);
	if (velocity.Length2() > MAX_SPEED_2) {								// Addjust maximume speed.
		velocity.Normalize();
		velocity.Multiply(MAX_SPEED);
	}
	pos = pos + (dt * velocity);
}
