#pragma once
#include "Vec3.h"
#include <stdint.h>
#include "Colors.h"
#include "Scene.h"

#include "Shape.h"
#define BOUNCE_DEPTH 2

class Ray
{
public:
	Vec3 src,dst;
	uint8_t bounces = 0;
	Ray(const Vec3& src, const Vec3& dst) :src(src), dst(dst), bounces() {};
	~Ray() {};
	Color raytrace(const Scene& scene, Shape* shapeHit)
	{
		Color finalColor = Colors::Black;
		float minDist = INFINITY;
		for (Shape* shp : scene.shapes)
		{
			if (shp == shapeHit) continue;
			float dist = shp->distance(*this);
			if (dist < minDist)
			{
				minDist = dist;
				shapeHit = shp;
			}
		}
		if (minDist != INFINITY)
		{
			Vec3 pHit = dst*minDist + src;
			finalColor = shapeHit->getColor(*this, pHit, scene);
			if (bounces++ < BOUNCE_DEPTH)
			{
				Vec3 N = shapeHit->getNormal(pHit);
				Vec3 V = -dst*minDist; // toViewer
				Vec3 Rv = (N*(V*N) * 2 - V).GetNormalized(); // toViewerReflected
				src = pHit;
				dst = Rv;
				Color color = this->raytrace(scene, shapeHit);
				color.SetLight(shapeHit->ks);
				finalColor += color;
			}
		}
		return finalColor;
	};
};

