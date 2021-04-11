#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>

/// Forward declerations
class Ant;
class Food;
class FoodPile;
class Pheromone;
class Vector2D;



bool InitGraphics();
void CleanupGraphics();
void ClearBackground();
void UpdateScreen();
void SetDrawColor(int r, int g, int b);
void DrawBorder();

void DrawAnt(const Ant& a);
void DrawFood(const Food& f);
void DrawFoodPile(const FoodPile& fp);
void DrawHome(const Vector2D& home_pos);
void DrawPheromone(const Pheromone& p);

void DrawCircle(int cx, int cy, int r);
void DrawFillCircle(int cx, int cy, int r);
void DrawString(const char* msg, int x, int y);
void DrawString(const std::string& msg, int x, int y);

#endif
