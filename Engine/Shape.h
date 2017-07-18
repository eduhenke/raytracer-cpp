#pragma once
#include "Vec3.h"
#include "Scene.h"
#include "Colors.h"
#define AMBIENT_LIGHT 0.1


class Ray;
class Shape
{
public:
	float ka = 0.6, ks = 0.4, kd = 0.6, alpha = 40.;
	Color color;
	Shape() : color(Colors::White) {};
	Shape(const Color& c) : color(c) {};
	virtual Vec3 getCenter() = 0;
	virtual Vec3 getNormal(const Vec3& pHit) = 0;
	virtual double distance(const Ray& ray)
	{
		return INFINITY;
	};
	virtual bool intersects(const Ray& ray)
	{
		return (distance(ray) == INFINITY) ? false : true;
	}
	Color getColor(Ray& viewerRay, const Vec3& pHit, const Scene& scene);
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
	double distance(const Ray& ray);
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
	double distance(const Ray& ray);
};
