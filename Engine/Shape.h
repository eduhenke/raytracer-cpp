#pragma once
#include "Vec3.h"

class Shape
{
public:
	virtual Vec3 getCenter() = 0;
	virtual bool intersects(const Vec3& src, const Vec3& dest) = 0;
};

class Sphere : public Shape
{
	float r = 0;
	Vec3 center = { 0,0,0 };
public:
	Sphere(const Vec3& c, float r) : center(c), r(r) {};
	Vec3 getCenter()
	{
		return center;
	}
	bool intersects(const Vec3& src, const Vec3& dest)
	{
		return true;
	}
};

