#pragma once
#include "Vec3.h"
#include <stdint.h>
#include "Colors.h"
#include "Scene.h"

#include "Shape.h"
#include <assert.h>
#define MAX_RAY_DEPTH 6
#define UNIT_VECTOR_EPSILON 1e-5
#define SHADOW_COLOR Colors::Black
#define SKY_COLOR Colors::SkyBlue

class Ray
{
public:
	Vec3 src,dst;
	Ray(const Vec3& src, const Vec3& dst) :src(src), dst(dst)
	{
		assert(dst*dst - 1. < UNIT_VECTOR_EPSILON);
	};
	~Ray() {};
	Color raytrace(const Scene& scene, Shape* shapeHit, Shape* prevShape, uint8_t bounces);
};

