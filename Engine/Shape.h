#pragma once
#include "Vec3.h"

class Shape
{
public:
	Color color = Colors::White;
	virtual Vec3 getCenter() = 0;
	virtual float distance(const Vec3& src, const Vec3& dest) = 0;
	virtual bool intersects(const Vec3& src, const Vec3& dest) = 0;
};

class Sphere : public Shape
{
public:
	float r = 0;
	Vec3 center = { 0,0,0 };
	Sphere(const Vec3& c, float r) : center(c), r(r) {};
	Vec3 getCenter()
	{
		return center;
	}
	float distance(const Vec3& o, const Vec3& d)
	{
		Vec3 dist = o - center;
		float b = 2 * (d*dist);
		float c = sq(dist) - sq(r);
		float det = sq(b) - 4 * c;			// a = d^2, d being unit vector => a = 1
		if (det < 0) return INFINITY;
		float sqr = sqrt(det);
		float t1 = -b - sqr;
		float t2 = -b + sqr;
		if (t1 < 0) return INFINITY;
		float t = min(t1, t2);
		return (t < 0) ? INFINITY : t;

	}
	bool intersects(const Vec3& o, const Vec3& d)
	{
		return (distance(o, d) == INFINITY) ? false : true;
	}
};

