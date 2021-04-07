#ifndef PHEROMONE_H_
#define PHEROMONE_H_

#include <list>

#include "vector2d.h"

class Pheromone
{
public:
	Pheromone() { Init(); }
	Pheromone(const Vector2D& pos) : pos(pos), strength(PHEROMONE_STRENGTH) {}
	Pheromone(const Pheromone& o) : pos(o.pos), strength(o.strength) {}

	Pheromone& operator=(const Pheromone& o)
	{
		pos = o.pos;
		strength = o.strength;
	}

	inline void Init()
	{
		pos.Init();
		strength = 0.0;
	}

	inline void Update(double dt)
	{
		// Decrease strength.
		strength -= dt * PHEROMONE_DECREASE_RATE;
	}

	Vector2D pos;
	double strength;

};

typedef std::list<Pheromone> PheromoneList;

#endif
