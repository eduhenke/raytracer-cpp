#pragma once
#include "Vec3.h"

class Shape
{
public:
	Color color = Colors::White;
	virtual Vec3 getCenter() = 0;
	virtual Vec3 getNormal(const Vec3& pHit) = 0;
	virtual double distance(const Vec3& src, const Vec3& dest) = 0;
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
	Vec3 getNormal(const Vec3& pHit)
	{
		
		return (pHit - center) / r;	// can be optimized to / r;
	}
	double distance(const Vec3& o, const Vec3& d)
	{
		Vec3 dist = o - center;
		double b = 2 * (d*dist);
		double c = sq(dist) - sq(r);
		double det = sq(b) - 4 * c;			// a = d^2, d being unit vector => a = 1
		if (det < 0.) return INFINITY;
		det = sqrt(det);
		double t = (-b - det)/2;
		if (t < 0.) return INFINITY;
		double t2 = (-b + det) / 2;
		t = min(t, t2);
		return (t < 0.) ? INFINITY : t;

	}
	bool intersects(const Vec3& o, const Vec3& d)
	{
		return (distance(o, d) == INFINITY) ? false : true;
	}
};

