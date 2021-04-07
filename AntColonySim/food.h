#ifndef FOOD_H_
#define FOOD_H_

#include <vector>

#include "constants.h"
#include "vector2d.h"

class Food
{
public:
	Food() {}
	Food(double x, double y) : pos(x, y) {}
	Food(const Food& f) : pos(f.pos) {}

	inline Food& operator=(const Food& f)
	{
		pos = f.pos;
		return *this;
	}

	Vector2D pos;
};

typedef std::vector<Food> FoodVect;


class FoodPile
{
public:
	FoodPile(double x, double y, int width, int height)
		: width(width)
		, height(height)
		, pos(x, y)
	{
		for (int j = 0; j < height; ++j)
			for (int i = 0; i < width; ++i)
				foods.push_back(Food(x + (i * FOOD_WIDTH), y + (j * FOOD_HEIGHT)));
	}

	FoodPile(const FoodPile& o)
		: width(o.width)
		, height(o.height)
		, pos(o.pos)
		, foods(o.foods)
	{}

	inline bool CheckCollision(const Vector2D& p)
	{
		if ((pos.x - SENSOR_RADIUS) <= p.x
				&& p.x <= (pos.x + (width * FOOD_WIDTH) + SENSOR_RADIUS)
				&& (pos.y - SENSOR_RADIUS) <= p.y
				&& p.y <= (p.y + (height * FOOD_HEIGHT)) + SENSOR_RADIUS)
			for (unsigned i = 0; i < foods.size(); ++i)
				if (p.Distance2(foods[i].pos) < SENSOR_RADIUS_2) {
					foods.erase(foods.begin() + i);						// Remove food from world.
					return true;
				}

		return false;
	}

	inline bool IsEmpty() const { return foods.size() == 0; }

	int width;
	int height;
	Vector2D pos;
	FoodVect foods;
};

typedef std::vector<FoodPile> FoodPileVect;

#endif
