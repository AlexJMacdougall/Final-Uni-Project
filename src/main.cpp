/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <array>
#include <algorithm>
#include <iostream>

#include "LevelManager.hpp"
#include "SystemManager.hpp"
#include "Components/ScriptComponent.hpp"
#include "Components/PlayerController.hpp"
#include "Components/Vec2.hpp"


int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 720, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	//Set up Registry and SystemManager
	Registry REGISTRY;
	REGISTRY.RegisterComponent<Transform>();
	REGISTRY.RegisterComponent<Sprite>();
	REGISTRY.RegisterComponent<SphereCollider>();
	REGISTRY.RegisterComponent<BoxCollider>();
	REGISTRY.RegisterComponent<ScriptComponent>();

	LevelManager LEVEL(&REGISTRY);
	LEVEL.GenerateLevel(5);

	SystemManager SYSTEM(&REGISTRY,&LEVEL,1280,720);

	//Instantiate entities
	//Player
	Entity player = REGISTRY.CreateEntity();

	REGISTRY.AddComponent<Transform>(player, Transform{ Vector3{192,192,0},Quaternion{0},Vector3{1,1,0} });
	REGISTRY.AddComponent<Sprite>(player, Sprite{ {0,0,32,32},1,1 });
	REGISTRY.AddComponent<BoxCollider>(player, BoxCollider{ 32.0f,32.0f });

	PlayerController playerScript = PlayerController(player,SYSTEM.GetCamera(), &REGISTRY,&SYSTEM );
	REGISTRY.AddComponent<ScriptComponent>(player, ScriptComponent());
	REGISTRY.GetComponent<ScriptComponent>(player)->attachScript<PlayerController>(playerScript);

	SYSTEM.SetPlayer(player);
	LEVEL.SetPlayer(player);

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();
		SYSTEM.Update(dt);
		if (IsKeyPressed(KEY_UP))
		{
			LEVEL.Move("Up");
		}
		if (IsKeyPressed(KEY_DOWN))
		{
			LEVEL.Move("Down");
		}
		if (IsKeyPressed(KEY_LEFT))
		{
			LEVEL.Move("Left");
		}
		if (IsKeyPressed(KEY_RIGHT))
		{
			LEVEL.Move("Right");
		}
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}