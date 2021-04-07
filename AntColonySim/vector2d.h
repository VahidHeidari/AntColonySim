#ifndef VECTOR_2D_H_
#define VECTOR_2D_H_

#include <cmath>
#include <ostream>

class Vector2D
{
public:
	inline static double Dot(const Vector2D& a, const Vector2D& b)
	{
		double res = (a.x * b.x) + (a.y * b.y);
		return res;
	}

	inline static Vector2D Normalize(const Vector2D& a)
	{
		double len = a.Length();
		if (len == 0)
			return Vector2D();

		Vector2D res{ a.x / len, a.y / len };
		return res;
	}

	Vector2D() { Init();  }
	Vector2D(double x, double y) { Init(x, y); }
	Vector2D(const Vector2D& o) { Init(o.x, o.y); }
	Vector2D& operator=(const Vector2D& o) { return Init(o.x, o.y); }

	Vector2D& Init(double new_x=0.0, double new_y=0.0)
	{
		x = new_x;
		y = new_y;
		return *this;
	}

	inline double Length() const { return std::sqrt(Length2()); }
	inline double Length2() const { return (x * x) + (y * y); }
	inline Vector2D& Normalize() { *this = Normalize(*this); return *this; }
	inline Vector2D& Multiply(double s) { x *= s; y *= s; return *this; }
	inline double Dot(const Vector2D& v) const { return Dot(*this, v); }

	inline double Distance(const Vector2D& v) const { return std::sqrt(Distance2(v)); }
	inline double Distance2(const Vector2D& v) const
	{
		double dx = v.x - x;
		double dy = v.y - y;
		double res = (dx * dx) + (dy * dy);
		return res;
	}

	double x;
	double y;
};



inline static Vector2D operator+(const Vector2D& a, const Vector2D&b)
{
	Vector2D res{ a.x + b.x, a.y + b.y };
	return res;
}

inline static Vector2D operator-(const Vector2D& a, const Vector2D& b)
{
	Vector2D res{ a.x - b.x, a.y - b.y };
	return res;
}

inline static Vector2D operator*(double s, const Vector2D& v)
{
	Vector2D res{ s * v.x, s * v.y };
	return res;
}

inline static Vector2D operator/(double s, const Vector2D& v)
{
	return (1.0 / s) * v;
}

inline static std::ostream& operator<<(std::ostream& o, const Vector2D& v)
{
	o << '(' << v.x << ' ' << v.y << ')';
	return o;
}

#endif
