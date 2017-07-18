#pragma once
#include <vector>
#include "Vec3.h"

class Shape;

class Scene
{
public:
	std::vector<Vec3*> lights;
	std::vector<Shape*> shapes;
	Scene() {};
	Scene(const std::vector<Shape*> shps, const std::vector<Vec3*> lgts) :shapes(shps), lights(lgts) {};
	~Scene() {};
};

