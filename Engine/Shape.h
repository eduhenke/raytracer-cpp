#pragma once
#include "Vec3.h"

#define AMBIENT_LIGHT 0.2

class Shape
{
public:
	float ka = 0.6, ks = 0.4, kd = 0.6, alpha = 5.;
	Color color;
	Shape() : color(Colors::White) {};
	Shape(const Color& c) : color(c) {};
	virtual Vec3 getCenter() = 0;
	virtual Vec3 getNormal(const Vec3& pHit) = 0;
	virtual double distance(const Vec3& src, const Vec3& dest)
	{
		return INFINITY;
	};
	virtual bool intersects(const Vec3& o, const Vec3& d)
	{
		return (distance(o, d) == INFINITY) ? false : true;
	}
	Color getColor(const Vec3& pHit, const Vec3& eye, const Vec3& light, const std::vector<Shape*>& shapes)
	{
		Vec3 L = (light - pHit).GetNormalized(); // toLight
		Vec3 N = this->getNormal(pHit); //normal
		Vec3 R = (N*(L*N)*2-L).GetNormalized(); // reflectedLight
		Vec3 V = (eye - pHit).GetNormalized(); // toViewer
		float ln = L*N;
		float rv = R*V;
		rv = (rv < 0) ? 0 : rv;
		rv = pow(rv,alpha);
		float lightFactor = AMBIENT_LIGHT + ln*kd + rv*ks;
		if (lightFactor < AMBIENT_LIGHT)lightFactor = AMBIENT_LIGHT;
		for (int j = 0; j < shapes.size(); j++)
		{
			Shape* otherShp = shapes[j];
			if (otherShp == this) continue;
			if (otherShp->intersects(pHit, L)) lightFactor = 0.1;
		}
		Color color = this->color;
		color.SetLight(lightFactor);
		return color;

		

	};
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

class Sky : public Shape
{
public:
	Sky(const Color& c) : Shape(c) {};
	Vec3 getCenter() { return Vec3(INFINITY, INFINITY, INFINITY); };
	Vec3 getNormal(const Vec3& pHit) { return Vec3(0, 1, 0); };
};