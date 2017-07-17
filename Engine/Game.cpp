/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	sphere(Sphere(Vec3(0.5, 0.4, 1.4), 0.4)),
	plane(Sphere(Vec3(0.,-1e5, 0), 1e5))
{
	plane.color = Colors::Cyan;
	shapes.push_back(&plane);
	shapes.push_back(&sphere);
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	//theta -= 1e-3;
	//eye.y -= 1e-1;
	//sphere.center.z -= 1e-1;
	//light.x-=0.4;
	light.x += 0.2;
}

void Game::ComposeFrame()
{
	Mat3 rotationMatrix = Mat3::RotationY(theta);
	static double dx = 2. / WIDTH;
	static double dy = 2. / HEIGHT;
	for (float xn = -1; xn < 1 ; xn+=dx)
	{
		for (float yn = -1; yn < 1; yn+=dy)
		{
			/*float xn = (2 * x / (float)WIDTH ) - 1;
			float yn = 1 - (2 / (float)HEIGHT)*(y + 1);*/
			Vec3 to = Vec3(xn, yn, 0) - eye;
			to = to*rotationMatrix;
			to.Normalize();
			int x = (xn + 1)*WIDTH / 2;
			int y = HEIGHT*(1 - (yn + 1) / 2) - 1;
			for (int k = 0; k < shapes.size(); k++)
			{
				Shape* shp = shapes[k];
				float distance = shp->distance(eye, to);
				if (distance != INFINITY)
				{	
					Vec3 pointHit = to*distance + eye;
					Vec3 toLight = light - pointHit;
					toLight.Normalize();
					Vec3 normal = shp->getNormal(pointHit);
					float lightFactor = normal*toLight;
					if (lightFactor < 0.05) lightFactor = 0.05; // Ambient
					for (int j = 0; j < shapes.size(); j++)
					{
						if (j == k) continue;
						Shape* otherShp = shapes[j];
						if (otherShp->intersects(pointHit, toLight)) lightFactor = 0;
					}
					Color color = shp->color;
					color.SetR(color.GetR()*lightFactor);
					color.SetG(color.GetG()*lightFactor);
					color.SetB(color.GetB()*lightFactor);
					gfx.PutPixel(x, y, color);
				}
			}
		}

	}
}
