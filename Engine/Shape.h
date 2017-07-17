#pragma once
#include "Vec3.h"

class Shape
{
public:
	Color color = Colors::White;
	virtual Vec3 getCenter() = 0;
	virtual Vec3 getNormal(const Vec3& pHit) = 0;
	virtual double distance(const Vec3& src, const Vec3& dest) = 0;
	virtual bool intersects(const Vec3& o, const Vec3& d)
	{
		return (distance(o, d) == INFINITY) ? false : true;
	}
};

class Sphere : public Shape
{
public:
	float r;
	Vec3 center;
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
};

class Plane : public Shape
{
public:
	Vec3 normal;
	float dist;
	Plane(const Vec3& normal, const Vec3& pos): normal(normal), dist(pos.Len())	{};
	Plane(const Vec3& normal, float dist): normal(normal), dist(dist) {};
	Vec3 getCenter()
	{
		return normal*dist;
	};
	Vec3 getNormal(const Vec3& pHit)
	{
		return normal;
	}
	double distance(const Vec3& o, const Vec3& d)
	{
		double t = dist - (o*normal) / (d*normal);
		return (t > 0) ? t : INFINITY;
	};
};