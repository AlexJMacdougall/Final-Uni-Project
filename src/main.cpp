/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "System.hpp"

#include <array>
#include <algorithm>
#include <iostream>

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	Registry test;
	test.RegisterComponent<Transform>();
	test.RegisterComponent<Texture>();

	Entity entity1 = test.CreateEntity();
	Entity entity2 = test.CreateEntity();

	test.AddComponent<Transform>(entity1, Transform{ Vector3{500,400,0},Quaternion{0},Vector3{1} });
	test.AddComponent<Texture>(entity1,Texture{ LoadTexture("wabbit_alpha.png") });

	test.AddComponent<Transform>(entity2, Transform{ Vector3{100,200,0},Quaternion{0},Vector3{1} });
	test.AddComponent<Texture>(entity2, Texture{ LoadTexture("wabbit_alpha.png") });

	DrawSystem Draw(std::make_shared<Registry>(test));
	Draw.AddEntities(test.GetEntitiesWithComponent<Texture>());

	PlayerController playerController(std::make_shared<Registry>(test));
	playerController.AddEntities(std::set<Entity>{entity1});

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		playerController.Run(dt);

		Draw.Run(dt);
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

