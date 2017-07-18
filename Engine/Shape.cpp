#include "Shape.h"
#include "Ray.h"

Color Shape::getColor(Ray& viewerRay, const Vec3& pHit, const Scene& scene)
{
	Vec3 L = (*(scene.lights[0]) - pHit); // toLight
	Vec3 L_hat = L.GetNormalized();
	Vec3 N = this->getNormal(pHit); //normal
	Vec3 V = -viewerRay.dst; // toViewer
	Vec3 H = (L_hat + V).GetNormalized(); // halfwayReflected
	float ln = L_hat*N;
	float nh = N*H;
	nh = pow(nh, alpha);
	float lightFactor = AMBIENT_LIGHT + ln*kd + nh*ks;
	float minDist = INFINITY; // min. distance of toLight for every Shape
	for (Shape* shp : scene.shapes)
	{
		if (shp == this)continue;
		float dist = shp->distance(Ray(pHit, L_hat));
		if (dist < minDist)
		{
			minDist = dist;
		}
	}
	if (minDist < L.Len()) lightFactor = 0;	// if minDist less than toLight, light is obstructed
	if (lightFactor < AMBIENT_LIGHT)lightFactor = AMBIENT_LIGHT;
	return this->color*lightFactor;

}

double Sphere::distance(const Ray & ray)
{
	Vec3 dist = ray.src - center;
	double b = 2 * (ray.dst*dist);
	double c = sq(dist) - sq(r);
	double det = sq(b) - 4 * c;			// a = d^2, d being unit vector => a = 1
	if (det < 0.) return INFINITY;
	det = sqrt(det);
	double t = (-b - det) / 2;
	if (t < 0.) return INFINITY;
	double t2 = (-b + det) / 2;
	t = min(t, t2);
	return (t < 0.) ? INFINITY : t;
}

double Plane::distance(const Ray & ray)
{
	double t = dist - (ray.src*normal) / (ray.dst*normal);
	return (t > 0) ? t : INFINITY;
};
