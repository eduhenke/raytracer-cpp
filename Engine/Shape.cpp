#include "Shape.h"
#include "Ray.h"

double Sphere::distance(const Ray & ray)
{
	Vec3 dist = ray.src - center;
	double b = 2 * (ray.dst*dist);
	double c = sq(dist) - rSq;
	double disc = sq(b) - 4 * c;			// a = d^2, d being unit vector => a = 1
	if (disc < 0.) return INFINITY;
	disc = sqrt(disc);
	double t = (-b - disc) / 2;
	if (t >= 0) return t;
	t += disc;
	return t < 0 ? INFINITY : t;
}

double Plane::distance(const Ray & ray)
{
	double t = dist - (ray.src*normal) / (ray.dst*normal);
	return (t > 0) ? t : INFINITY;
};

Color Shape::getDiffuseColor(const Vec3 & pHit, const Scene & scene)
{
	float finalLightFactor = 0;
	Vec3 N = this->getNormal(pHit); //normal
	float lightsSize = scene.lights.size();
	for (Vec3* lgt : scene.lights)
	{
		Vec3 L = lgt->GetNormalized();
		//Vec3 V = -viewerRay.dst; // toViewer
		//Vec3 H = (L + V).GetNormalized(); // halfwayReflected
		//float nh = N*H;
		//nh = pow(nh, alpha);
		float lightFactor = AMBIENT_LIGHT + L*N*diffuse;// +nh*reflective;
		double minDist = INFINITY; // min. distance of toLight for every Shape
		for (Shape* shp : scene.shapes)
		{
			if (shp == this)continue;
			minDist = min(minDist, shp->distance(Ray(pHit, L)));
		}
		if (minDist < lgt->Len()) lightFactor = 0;//finalColor += SHADOW_COLOR;	// if minDist less than toLight, light is obstructed
		else if (lightFactor < AMBIENT_LIGHT) lightFactor = AMBIENT_LIGHT;
		finalLightFactor += (lightFactor / lightsSize); // divided by how many lights
	}
	return color*finalLightFactor;
}