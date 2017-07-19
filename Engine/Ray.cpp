#include "Ray.h"
#include <algorithm>

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	return clamp(v, lo, hi, std::less<>());
}

Vec3 refract(const Vec3 &I, const Vec3 &N, const float &ior)
{
	float cosi = (I * N);
	if (cosi > 1.)cosi = 1.;
	if (cosi < -1.)cosi = -1.;
	float etai = 1, etat = ior;
	Vec3 n = N;
	if (cosi < 0) { cosi = -cosi; }
	else { std::swap(etai, etat); n = -N; }
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? Vec3(0,0,0): (I * eta + n*(eta * cosi - sqrtf(k))); // null vec can be wrong
}

Color Ray::raytrace(const Scene & scene, Shape * shapeHit, Shape * prevShape, uint8_t bounces)
{
	assert(dst*dst - 1. < UNIT_VECTOR_EPSILON);
	if (shapeHit != NULL) prevShape = shapeHit;
	float minDist = INFINITY;
	for (Shape* shp : scene.shapes)
	{
		//if (shp == shapeHit) continue;
		float dist = shp->distance(*this);
		if (dist < minDist)
		{
			minDist = dist;
			shapeHit = shp;
		}
	}
	if (minDist == INFINITY || shapeHit == NULL) return SKY_COLOR;
	else
	{
		Vec3 I = dst*minDist;
		Vec3 pHit = I + src;
		I.Normalize();
		Color diffuseColor = shapeHit->getDiffuseColor(pHit, scene);
		if (!shapeHit->transparency && shapeHit->diffuse == 1) // no need to cast rays
		{
			return diffuseColor;//shapeHit->diffuse is 1
		}
		// finished bouncing
		else if (bounces + 1 >= MAX_RAY_DEPTH)
		{
			return diffuseColor*shapeHit->diffuse;
		}
		// get ready to cast a ray
		else
		{
			Vec3 N = shapeHit->getNormal(pHit);			  // Normal
			Vec3 R = (I - N*(I*N) * 2).GetNormalized();   // Reflected
			Vec3 offset = N*0.001;
			Color transmitColor = SKY_COLOR, reflectColor = SKY_COLOR;

			// if solid and glossy(1 ray)
			if (shapeHit->transparency == 0 && shapeHit->reflective > 0)
			{
				src = pHit + offset;	// ray source nudged out
				dst = R;				// ray destination is the Reflected ray
				reflectColor = this->raytrace(scene, shapeHit, prevShape, bounces + 1);
				return reflectColor*shapeHit->reflective + diffuseColor*shapeHit->diffuse;
			}
			// if transparent(1-2 rays)
			else
			{
				// if totally transparent(1 ray)
				if (shapeHit->reflective == 0)
				{
					src = shapeHit->inside(pHit + I*0.001) ? pHit - offset : pHit + offset;
					dst = refract(I, N, shapeHit->ior).GetNormalized(); // if ior = 1
					transmitColor = this->raytrace(scene, shapeHit, prevShape, bounces + 1);
					return transmitColor*shapeHit->transparency;//+ diffuseColor*shapeHit->diffuse;
				}
				// if transparent and reflective(2 rays)
				else
				{
					//src = pHit - N*0.001; // nudged in
					//dst = T;
					//transmitColor = this->raytrace(scene, shapeHit, prevShape, bounces + 1) * shapeHit->diffuse;
					src = pHit + offset;	// ray source nudged in
					dst = R;				// ray destination is the Reflected ray
					reflectColor = this->raytrace(scene, shapeHit, prevShape, bounces + 1);
					return reflectColor*shapeHit->reflective + diffuseColor*shapeHit->diffuse;
				}
			}
		}
	}
};