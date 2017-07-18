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
	gfx( wnd )
{
	floor.color = Colors::Green;
	sphere2.color = Colors::Red;
	shapes.push_back(&floor);
	shapes.push_back(&sphere2);
	shapes.push_back(&sphere);
	shapesSize = shapes.size();
}

inline Vec3 RotateYThroughAxis(const Vec3& vec, const Vec3& axis, float theta)
{
	return axis + (vec - axis)*Mat3::RotationY(theta);
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
	while (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		static float step = 0.5;
		static float thetaStep = 0.1;
		if (wnd.kbd.KeyIsPressed('W'))
		{
			//thetaY += thetaStep;
			light.z++;
		}
		else if (wnd.kbd.KeyIsPressed('S'))
		{
			//thetaY -= thetaStep;
			light.z--;
		}
		else if (wnd.kbd.KeyIsPressed('A'))
		{
			//thetaX -= thetaStep;
			light.x--;
		}
		else if (wnd.kbd.KeyIsPressed('D'))
		{
			//thetaX += thetaStep;
			light.x++;
		}
		else if (wnd.kbd.KeyIsPressed('Q'))
		{
			light.y++;
		}
		else if (wnd.kbd.KeyIsPressed('E'))
		{
			light.y--;
		}
		else if (e.GetCode() == VK_ESCAPE && e.IsPress())
		{
			wnd.Kill();
		}
	}
}

void Game::ComposeFrame()
{
	Mat3 rotationMatrix = Mat3::RotationY(thetaX)*Mat3::RotationX(thetaY);
	static float dx = 2. / WIDTH;
	static float dy = 2. / HEIGHT;
	for (float xn = -1.; xn < 1.; xn += dx)
	{
		for (float yn = -1.; yn < 1.; yn += dy)
		{
			/*float xn = (2 * x / (float)WIDTH ) - 1;
			float yn = 1 - (2 / (float)HEIGHT)*(y + 1);*/
			Vec3 to = (Vec3(xn, yn, 0) - eye)*rotationMatrix;
			to.Normalize();
			int x = (xn + 1)*WIDTH / 2;
			int y = HEIGHT*(1 - (yn + 1) / 2) - 1;
			float minDist = INFINITY;
			for (Shape* shp : shapes)
			{
				float dist = shp->distance(eye, to);
				if (minDist >= dist)
				{
					minDist = dist;
					shapeHitScreen[x][y] = shp;
				}
			}
		}
	}
	for (float xn = -1.; xn < 1. ; xn+=dx)
	{
		for (float yn = -1.; yn < 1.; yn+=dy)
		{
			Vec3 to = (Vec3(xn, yn, 0) - eye)*rotationMatrix;
			to.Normalize();
			int x = (xn + 1)*WIDTH / 2;
			int y = HEIGHT*(1 - (yn + 1) / 2) - 1;
			Shape* shapeHit = shapeHitScreen[x][y];
			float distance = shapeHit->distance(eye, to);
			if (distance != INFINITY)
			{
				Vec3 pointHit = to*distance + eye;
				Color color = shapeHit->getColor(pointHit, eye, light, shapes);
				gfx.PutPixel(x, y, color);
			}
		}

	}
}
