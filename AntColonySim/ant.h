#ifndef ANT_H_
#define ANT_H_

#include <vector>

#include "ring-buffer.h"
#include "vector2d.h"

enum class AntState
{
	AS_WANDER,
	AS_CARRY_FOOD,
};

class Ant
{
public:
	Ant() { Init(); }
	Ant(double x, double y) { Init(); pos.Init(x, y); }

	void Init();
	inline void ChangeState(AntState new_state) { state = new_state; }
	inline bool IsState(AntState chk_state) const { return state == chk_state; }
	void Update(double dt);

	AntState state;
	double force_update_time;
	double trail_update_time;
	double pheromone_update_time;
	double mass;
	Vector2D pos;
	Vector2D velocity;
	Vector2D acceleration;
	RingBuffer<Vector2D> trail;
};

typedef std::vector<Ant> AntVect;

#endif
