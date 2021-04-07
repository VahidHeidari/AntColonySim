#ifndef FOOD_H_
#define FOOD_H_

#include <vector>

#include "vector2d.h"

class Food
{
public:
	Food() {}
	Food(double x, double y) : pos(x, y) {}
	Food(const Food& f) : pos(f.pos) {}

	Food& operator=(const Food& f)
	{
		pos = f.pos;
		return *this;
	}

	Vector2D pos;
};

typedef std::vector<Food> FoodVect;

#endif
