#pragma once

struct Vec2
{
	double x;
	double y;

	Vec2() = default;

	Vec2(double _x, double _y)
		: x(_x)
		, y(_y) {}

	inline double length() const	// ’·‚³
	{
		return sqrt(x * x + y * y);
	}

	inline double lengthSquare() const // ’·‚³‚Ì“ñæ
	{
		return x * x + y * y;
	}

	inline double dot(const Vec2& a) const // “àÏ
	{
		return x * a.x + y * a.y;
	}

	inline double distance(const Vec2& a) const    // ‹——£
	{
		return sqrt((a.x - x) * (a.x - x) + (a.y - y) * (a.y - y));
	}

	inline Vec2 normalized() const // ³‹K‰»
	{
		double	_len = length();
		return{ x / _len , y / _len };
	}

	inline bool isZero() const // ƒ[ƒƒxƒNƒgƒ‹‚Å‚ ‚é‚©
	{
		return x == 0.0 && y == 0.0;
	}

	inline void clear(double a)
	{
		x = a;
		y = a;
	}

	inline Vec2 operator +() const
	{
		return *this;
	}

	inline Vec2 operator -() const
	{
		return{ -x, -y };
	}

	inline Vec2 operator +(const Vec2& other) const
	{
		return{ x + other.x, y + other.y };
	}

	inline Vec2 operator -(const Vec2& other) const
	{
		return{ x - other.x, y - other.y };
	}

	inline Vec2 operator *(double s) const
	{
		return{ x * s, y * s };
	}

	inline Vec2 operator /(double s) const
	{
		return{ x / s, y / s };
	}

	inline Vec2& operator +=(const Vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Vec2& operator -=(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline Vec2& operator *=(double s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	inline Vec2& operator /=(double s)
	{
		x /= s;
		y /= s;
		return *this;
	}
};

inline Vec2 operator *(double s, const Vec2& v) // Vec2 ‚ªŒã‚É‚­‚é 2€ *
{
	return{ s * v.x, s * v.y };
}