#pragma once
#include "Vec3.h"
#include "Scene.h"
#include "Colors.h"
#define AMBIENT_LIGHT 0.1


class Ray;
class Shape
{
public:
	float transparency, ior, reflective, diffuse, alpha;
	Color color;
	Shape() : Shape(0., 0.2) {};
	Shape(const Color& c) :Shape(c, 0., 0.2) {};
	Shape(float trsp, float ref) : Shape(trsp, ref, 1.) {};
	Shape(float trsp, float ref, float ior) : Shape(Colors::White, trsp, ref, ior) {};
	Shape(const Color& c, float trsp, float ref) : Shape(c, trsp, ref, 1) {};
	Shape(const Color& c, float trsp, float ref, float ior) : Shape(c, trsp, ref, ior, 40.) {};
	Shape(const Color& c, float trsp, float refl, float ior, float alpha)
		: color(c), transparency(trsp), reflective(refl), diffuse(1-refl), ior(ior), alpha(alpha) {};
	virtual Vec3 getCenter() = 0;
	virtual Vec3 getNormal(const Vec3& pHit) = 0;
	virtual double distance(const Ray& ray)
	{
		return INFINITY;
	};
	virtual bool intersects(const Ray& ray)
	{
		return (distance(ray) == INFINITY) ? false : true;
	};
	virtual bool inside(const Vec3& p)
	{
		return false;
	};
	Color getDiffuseColor(const Vec3& pHit, const Scene& scene);
};

class Sphere : public Shape
{
public:
	float r, rSq; // rSq = r*r
	Vec3 center;
	Sphere(const Vec3& c, float r) : center(c), r(r), rSq(r*r), Shape() {};
	Sphere(const Vec3& c, float r, float trsp, float ref, float ior) : center(c), r(r), rSq(r*r), Shape(trsp, ref, ior) {};
	Sphere(const Vec3& c, float r, const Color& col, float trsp, float ref, float ior) : center(c), r(r), rSq(r*r), Shape(col, trsp, ref, ior) {};
	Sphere(const Vec3& c, float r, const Color& col) : center(c), r(r), rSq(r*r), Shape(col) {};
	Vec3 getCenter()
	{
		return center;
	}
	Vec3 getNormal(const Vec3& pHit)
	{
		
		return (pHit - center) / r;
	}
	double distance(const Ray& ray);
	bool inside(const Vec3& p)
	{
		return sq(p - center) < rSq;
	};
};

class Plane : public Shape
{
public:
	Vec3 normal;
	float dist;
	Plane(const Vec3& normal, const Vec3& pos): normal(normal), dist(pos.Len())	{};
	Plane(const Vec3& normal, float dist): normal(normal), dist(dist) {};
	Plane(const Vec3& normal, float dist, const Color& col) :normal(normal), dist(dist), Shape(col) {};
	Vec3 getCenter()
	{
		return normal*dist;
	};
	Vec3 getNormal(const Vec3& pHit)
	{
		return normal;
	}
	double distance(const Ray& ray);
	bool inside(const Vec3& p)//shouldnt be used...
	{
		return false;
	};
};
