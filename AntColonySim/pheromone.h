#ifndef PHEROMONE_H_
#define PHEROMONE_H_

#include <list>
#include <vector>

#include "constants.h"
#include "graphics.h"
#include "vector2d.h"

class Pheromone
{
public:
	Pheromone() { Init(); }
	Pheromone(const Vector2D& pos) : pos(pos), strength(PHEROMONE_STRENGTH) {}
	Pheromone(const Pheromone& o) : pos(o.pos), strength(o.strength) {}

	inline Pheromone& operator=(const Pheromone& o)
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

class PheromoneGrid
{
public:
	typedef std::vector<PheromoneList> PheromoneListGrid;

	PheromoneGrid() : rows(0), cols(0), cell_width_px(0), cell_height_px(0), grid_width(0), grid_height(0) {}

	inline void Init(int rows, int cols, int cell_width_px=CELL_WIDTH_PX, int cell_height_px=CELL_HEIGHT_PX)
	{
		this->rows = rows;
		this->cols = cols;
		this->cell_width_px = cell_width_px;
		this->cell_height_px = cell_height_px;

		grid_width = cell_width_px * cols;
		grid_height = cell_height_px * rows;
		grid.clear();
		grid.resize(rows * cols);
	}

	inline bool AddPheromone(const Pheromone& p)
	{
		if (IsOutOfGridBoundary(p.pos))
			return false;

		grid[GetIdx(p.pos)].push_back(p);
		return true;
	}

	inline void Draw() const
	{
		for (unsigned i = 0; i < grid.size(); ++i)
			std::for_each(grid[i].begin(), grid[i].end(), [&](const auto& p) { DrawPheromone(p); });
	}

	inline const PheromoneList* GetNearByPheromoneList(Vector2D& pos) const
	{
		if (IsOutOfGridBoundary(pos))
			return nullptr;

		const int idx = GetIdx(pos);
		if (grid[idx].size() == 0)
			return nullptr;

		return &grid[idx];
	}

	inline void Update(double dt)
	{
		for (unsigned i = 0; i < grid.size(); ++i) {
			PheromoneList& pheromones = grid[i];
			for (PheromoneList::iterator p = pheromones.begin(); p != pheromones.end();) {
				PheromoneList::iterator next = p;
				++next;
				p->Update(dt);
				if (p->strength < 0)
					pheromones.erase(p);
				p = next;
			}
		}
	}

private:
	inline bool IsOutOfGridBoundary(const Vector2D& pos) const
	{
		return pos.x < 0.0 || grid_width < pos.x
			|| pos.y < 0.0 || grid_height < pos.y;
	}

	inline int GetIdx(const Vector2D& pos) const
	{
		return (static_cast<int>(pos.y / cell_height_px) * cols) + static_cast<int>(pos.x / cell_width_px);
	}

	int rows;
	int cols;
	int cell_width_px;
	int cell_height_px;
	double grid_width;
	double grid_height;
	PheromoneListGrid grid;
};

#endif
