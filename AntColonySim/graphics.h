#ifndef GRAPHICS_H_
#define GRAPHICS_H_

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

#endif
