/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Ray.h"
#include "Mat3.h"
#include <cstdlib>
#include <ctime>
#define HEIGHT Graphics::ScreenHeight
#define WIDTH Graphics::ScreenWidth
class Game
{
public:
	Game(class MainWindow& wnd);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/
	float thetaY = -5e-1, thetaX = 0;
	Vec3 eye = { 0, 1.7, -1.5 };
	Vec3 light = { 0.5,5.,3.2 };
	Vec3 light2 = { 10,50.,100 };
	Vec3 light3 = { 0.2,8.,-10.0 };
	Scene scene;
	Sphere sphere =  Sphere(Vec3(0.2, 0.8, 5.8), 1, Colors::HotPink, 0.8, 0., 4);
	Sphere sphere2 = Sphere(Vec3(4., 0., 45.), 30., Colors::Blue, 0, 0.1, 1);
	Sphere sphere3 = Sphere(Vec3(-5., 4., 10.), 4., Colors::Green, 0, 0.3, 1);
	Plane floor =	 Plane(Vec3(0., 1, 0), 0, Colors::Red);
};