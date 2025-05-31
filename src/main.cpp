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
#include "Components/MeleeEnemyScript.hpp"
#include "Components/Vec2.hpp"

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 720, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	//Set up Registry and SystemManager
	Registry REGISTRY;
	REGISTRY.RegisterComponent<Transform2D>();
	REGISTRY.RegisterComponent<Navmesh>();
	REGISTRY.RegisterComponent<Sprite>();
	REGISTRY.RegisterComponent<AnimatedSprite>();
	REGISTRY.RegisterComponent<SphereCollider>();
	REGISTRY.RegisterComponent<BoxCollider>();
	REGISTRY.RegisterComponent<ScriptComponent>();

	LevelManager LEVEL(&REGISTRY);
	LEVEL.GenerateLevel(10);

	SystemManager SYSTEM(&REGISTRY, &LEVEL, 1280, 720);

	//Instantiate entities
	//Player
	Entity player = REGISTRY.CreateEntity();

	REGISTRY.AddComponent<Transform2D>(player, Transform2D{ Vec2{150,150},Vec2{1,1} });
	REGISTRY.AddComponent<BoxCollider>(player, BoxCollider{ 32.0f,32.0f });

	Sprite playerSprite = Sprite{ {0,0},"Player",2 };
	REGISTRY.AddComponent<Sprite>(player, playerSprite);
	AnimatedSprite playerAnimations = AnimatedSprite{ &playerSprite,0.2f };
	playerAnimations.animationData["Idle"] = Animation{Vec2{0,0},5};
	playerAnimations.animationData["Walk"] = Animation{Vec2{0,1},5};
	playerAnimations.currentAnimation = "Idle";
	REGISTRY.AddComponent<AnimatedSprite>(player, playerAnimations);

	PlayerController playerScript = PlayerController(player, SYSTEM.GetCamera(), &REGISTRY);
	REGISTRY.AddComponent<ScriptComponent>(player, ScriptComponent());
	REGISTRY.GetComponent<ScriptComponent>(player)->attachScript<PlayerController>(playerScript);
	
	SYSTEM.SetPlayer(player);
	LEVEL.SetPlayer(player);

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();
		SYSTEM.Update(dt);
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}