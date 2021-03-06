#include "graphics.h"

#include <algorithm>

#include <SDL.h>

#include "ant.h"
#include "constants.h"
#include "font.h"
#include "food.h"
#include "pheromone.h"

#ifdef _MSC_VER
#pragma comment(lib, "SDL2.lib")
#endif

static const Uint32 INIT_FLAGS = SDL_INIT_VIDEO;

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_DisplayMode display_mode;

static SDL_Rect ant_rect;
static SDL_Rect food_rect;
static SDL_Rect pheromone_rect;

bool is_draw_debugs_circle = false;
bool is_draw_debugs_bounding_box = false;
bool is_draw_trails = false;

bool InitGraphics()
{
	if (SDL_InitSubSystem(INIT_FLAGS) < 0)
		return false;

	const int SHOW_FLAGS = SDL_WINDOW_SHOWN |
		(IS_FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0);

	window = SDL_CreateWindow(
		WINDOW_TITLE,				// Caption
		WINDOW_X,					// X
		WINDOW_Y,					// Y
		WINDOW_WIDTH,				// Width
		WINDOW_HEIGHT,				// Height
		SHOW_FLAGS);				// Flags
	if (!window)
		return false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		return false;

	display_mode.format = SDL_PIXELFORMAT_RGB888;
	display_mode.w = WINDOW_WIDTH;
	display_mode.h = WINDOW_HEIGHT;
	display_mode.refresh_rate = 0;
	display_mode.driverdata = nullptr;
	if (SDL_SetWindowDisplayMode(window, &display_mode) < 0)
		return false;

	//SetWindowIcon(ICON_PATH);

	ant_rect.w = static_cast<int>(SENSOR_RADIUS);
	ant_rect.h = static_cast<int>(SENSOR_RADIUS);
	food_rect.w = FOOD_WIDTH;
	food_rect.h = FOOD_HEIGHT;
	pheromone_rect.w = PHEROMONE_WIDTH;
	pheromone_rect.h = PHEROMONE_HEIGHT;
	return true;
}

void CleanupGraphics()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_QuitSubSystem(INIT_FLAGS);
}

void ClearBackground()
{
	SetDrawColor(0, 0, 0);
	SDL_RenderClear(renderer);
}

void UpdateScreen()
{
	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}

void SetDrawColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer,
		static_cast<Uint8>(r),
		static_cast<Uint8>(g),
		static_cast<Uint8>(b), 255);
}

void DrawBorder()
{
	SetDrawColor(128, 128, 0);
	static const SDL_Rect brd_rect = {
		BORDER_MARGIN,
		BORDER_MARGIN,
		WINDOW_WIDTH - (2 * BORDER_MARGIN),
		WINDOW_HEIGHT - (2 * BORDER_MARGIN)
	};
	SDL_RenderDrawRect(renderer, &brd_rect);
}

void DrawAnt(const Ant& a)
{
	int px = static_cast<int>(a.pos.x);
	int py = static_cast<int>(a.pos.y);

	// Draw sensor circle.
	if (is_draw_debugs_circle) {
		SetDrawColor(127, 127, 255);
		DrawCircle(px, py, static_cast<int>(SENSOR_RADIUS));

		// Draw velocity vector.
		Vector2D v = a.velocity + a.pos;
		int vx = static_cast<int>(v.x);
		int vy = static_cast<int>(v.y);
		SetDrawColor(255, 0, 0);
		SDL_RenderDrawLine(renderer, px, py, vx, vy);
	}

	// Draw sensor bounding box.
	if (is_draw_debugs_bounding_box) {
		SetDrawColor(0, 255, 255);
		ant_rect.x = px - static_cast<int>(SENSOR_RADIUS);
		ant_rect.y = py - static_cast<int>(SENSOR_RADIUS);
		ant_rect.w = ant_rect.h = static_cast<int>(SENSOR_RADIUS * 2.0);
		SDL_RenderDrawRect(renderer, &ant_rect);
	}

	// Draw trail.
	if (is_draw_trails) {
		switch (a.state) {
		case AntState::AS_CARRY_FOOD:	SetDrawColor(255, 127, 0); break;
		case AntState::AS_WANDER:		SetDrawColor(127, 127, 255); break;
		}
		for (int i = a.trail.start; (i + 1) % a.trail.buffer.size() != a.trail.end; i = (i + 1) % a.trail.buffer.size()) {
			SDL_RenderDrawLine(renderer,
				static_cast<int>(a.trail.buffer[i].x),
				static_cast<int>(a.trail.buffer[i].y),
				static_cast<int>(a.trail.buffer[(i + 1) % a.trail.buffer.size()].x),
				static_cast<int>(a.trail.buffer[(i + 1) % a.trail.buffer.size()].y));
		}
	}

	// Draw direction.
	Vector2D norm = a.velocity.Length2() ? Vector2D::Normalize(a.velocity) : Vector2D(0, -1);
	Vector2D tip = 20 * norm + a.pos;
	Vector2D lft = -5 * Vector2D(norm.y, -norm.x) + (a.pos - 5 * norm);
	Vector2D rht = -5 * Vector2D(-norm.y, norm.x) + (a.pos - 5 * norm);
	SDL_Point pts[] = {
		{ static_cast<int>(lft.x),   static_cast<int>(lft.y) },
		{ static_cast<int>(tip.x),   static_cast<int>(tip.y) },
		{ static_cast<int>(rht.x),   static_cast<int>(rht.y) },
		{ px, py },
		{ static_cast<int>(lft.x),   static_cast<int>(lft.y) },
	};
	switch (a.state) {
	case AntState::AS_CARRY_FOOD:	SetDrawColor(128, 200, 0); break;
	case AntState::AS_WANDER:		SetDrawColor(10, 128, 128); break;
	}
	SDL_RenderDrawLines(renderer, pts, 5);
}

void DrawFood(const Food& p)
{
	food_rect.x = static_cast<int>(p.pos.x - FOOD_WIDTH / 2);
	food_rect.y = static_cast<int>(p.pos.y - FOOD_HEIGHT / 2);
	SetDrawColor(0, 128, 0);
	SDL_RenderFillRect(renderer, &food_rect);
	SetDrawColor(0, 255, 127);
	SDL_RenderDrawRect(renderer, &food_rect);
}

void DrawFoodPile(const FoodPile& fp)
{
	std::for_each(fp.foods.begin(), fp.foods.end(), [&](const auto& f) { DrawFood(f); });
}

void DrawHome(const Vector2D& home_pos)
{
	int hx = static_cast<int>(HOME_X);
	int hy = static_cast<int>(HOME_Y);
	SetDrawColor(0, 127, 255);
	DrawFillCircle(hx, hy, static_cast<int>(SENSOR_RADIUS));
	SetDrawColor(0, 0, 0);
	SDL_RenderDrawPoint(renderer, hx, hy);
}

void DrawPheromone(const Pheromone& p)
{
	pheromone_rect.x = static_cast<int>(p.pos.x - PHEROMONE_WIDTH / 2);
	pheromone_rect.y = static_cast<int>(p.pos.y - PHEROMONE_HEIGHT / 2);
	double s = p.strength / PHEROMONE_STRENGTH;
	double r = s * 230.0;
	double g = s * 10.0;
	double b = s * 128.0;
	SetDrawColor(static_cast<int>(r),
		static_cast<int>(g),
		static_cast<int>(b));
	SDL_RenderFillRect(renderer, &pheromone_rect);
}

void DrawCircle(int cx, int cy, int r)
{
	int x = r, y = 0, r2 = r * r;
	while (y <= x) {
		SDL_RenderDrawPoint(renderer, cx + x, cy + y);
		SDL_RenderDrawPoint(renderer, cx + y, cy + x);
		SDL_RenderDrawPoint(renderer, cx + x, cy - y);
		SDL_RenderDrawPoint(renderer, cx + y, cy - x);
		SDL_RenderDrawPoint(renderer, cx - x, cy + y);
		SDL_RenderDrawPoint(renderer, cx - y, cy + x);
		SDL_RenderDrawPoint(renderer, cx - x, cy - y);
		SDL_RenderDrawPoint(renderer, cx - y, cy - x);

		int e = 2 * ((x * x) + (y * y) - r2 + (2 * y + 1)) + (1 - 2 * x);
		if (e > 0)
			--x;
		y += 1;
	}
}

void DrawFillCircle(int cx, int cy, int r)
{
	int x = r, y = 0, r2 = r * r;
	while (y <= x) {
		SDL_RenderDrawLine(renderer, cx + x, cy + y, cx - x, cy + y);
		SDL_RenderDrawLine(renderer, cx + x, cy - y, cx - x, cy - y);
		SDL_RenderDrawLine(renderer, cx + y, cy + x, cx - y, cy + x);
		SDL_RenderDrawLine(renderer, cx + y, cy - x, cx - y, cy - x);

		int e = 2 * ((x * x) + (y * y) - r2 + (2 * y + 1)) + (1 + 2 * x);
		if (e > 0)
			--x;
		y += 1;
	}
}

void DrawString(const char* msg, int x, int y)
{
	int sx = x;
	for (; *msg != '\0'; ++msg) {
		if (*msg == '\n') {
			y += FONT_HEIGHT + 1;
			sx = x;
			continue;
		}

		if (*msg == '\r') {
			sx = x;
			continue;
		}

		if (*msg == ' ') {
			sx += FONT_WIDTH + 1;
			continue;
		}

		int idx = (*msg - ' ') * FONT_WIDTH;
		for (int j = 0; j < FONT_HEIGHT; ++j)
			for (int i = 0; i < FONT_WIDTH; ++i)
				if (font[idx + i] & (0x02 << j))
					SDL_RenderDrawPoint(renderer, sx + i, y + j);
		sx += FONT_WIDTH + 1;
	}
}

void DrawString(const std::string& msg, int x, int y)
{
	DrawString(msg.c_str(), x, y);
}
